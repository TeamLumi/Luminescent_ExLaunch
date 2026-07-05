#include "exlaunch.hpp"
#include "features/mp_log.h"
#include "features/mp_net.h"
#include "features/mp_contest.h"
#include "features/overworld_multiplayer.h"
#include "features/team_up.h"

#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/Dpr/Contest/ContestWork.h"
#include "externals/Dpr/Contest/EntryPlayerData.h"
#include "externals/System/String.h"

#include <cstdint>
#include <cstring>

// ---------------------------------------------------------------------------
// Multiplayer Super Contests — see mp_contest.h for the design overview.
//
// Flow (both consoles run vanilla SINGLE-mode contests):
//  1. Counter rendezvous arms the pairing (mp_counter.cpp).
//  2. Each player registers at the reception. EvCmdContestProc (the ev command
//     that commits the entry and launches the scene) is hooked: it builds the
//     local entry record early, sends it to the partner (0xD9), and stalls the
//     script until the partner's entry arrives (or times out -> solo).
//  3. Scene build: ContestDataModel.CreateNPCPlayerData is hooked for entrant
//     slot 1 — the NPC record is overwritten with the partner's real entry
//     (their Pokemon, contest costume, colors, conditions) and built through
//     vanilla's own remote-human builder (CreateOtherPlayerData), so the
//     partner appears ON STAGE. Their model dances via NPC AI locally.
//  4. Section boundary: ContestController.RequestChangeSectionId is hooked at
//     Dance -> PreLoadResult. Each console captures its own slot-0 scores
//     (visual/dance/appeal), sends them (0xD8), and stalls the transition
//     until the partner's scores arrive; those are injected into slot 1's
//     models so vanilla ranks all four entrants with REAL scores.
//  5. Rewards/records commit vanilla-side on each console.
//
// Game structures (Ghidra research 2026-07-04, mp-contest-research memory):
//   ContestController: currentSectionID @0x88, objectManagerPtr @0x40
//     (entity array @+0x38, elements @+0x20+k*8; entity+0x60 = the
//     AContestPlayerData: visual model @+0x40 / dance model @+0x48).
//   Scores: visual = visualModel+0x10 (heartNum), dance = danceModel+0x2C,
//     appeal/waza = danceModel+0x28.
//   SectionID: Opening=0 Visual=1 Dance=2 PreLoadResult=3 WaitAsync=4
//     Result=5 Restart=6 NetworkError=7 End=8.
// ---------------------------------------------------------------------------

// Game RVAs
static constexpr uintptr_t RVA_EVCMD_CONTEST_PROC       = 0x2C71A40; // EvCmdContestProc
static constexpr uintptr_t RVA_CREATE_NPC_PLAYER_DATA   = 0x1C6D7B0; // ContestDataModel.CreateNPCPlayerData
static constexpr uintptr_t RVA_CREATE_OTHER_PLAYER_DATA = 0x1C6DD10; // ContestDataModel.CreateOtherPlayerData
static constexpr uintptr_t RVA_REQUEST_CHANGE_SECTION   = 0x1C63720; // ContestController.RequestChangeSectionId

// ContestController field offsets
static constexpr uintptr_t CC_OBJECT_MANAGER  = 0x40;
static constexpr uintptr_t CC_CURRENT_SECTION = 0x88;

// SectionID values
static constexpr int32_t SECTION_DANCE          = 2;
static constexpr int32_t SECTION_PRELOAD_RESULT = 3;
static constexpr int32_t SECTION_RESTART        = 6;
static constexpr int32_t SECTION_END            = 8;

// Entry-wait: EvCmdContestProc re-runs each frame while we return "not done".
// ~30-60s at 30-60 fps before giving up and starting solo.
static constexpr int32_t CONTEST_ENTRY_WAIT_CALLS = 1800;
// Section-boundary safety timeout (ticked only if the field tick runs during
// the contest scene; the primary resume is 0xD8 receipt).
static constexpr float CONTEST_BOUNDARY_TIMEOUT_SEC = 60.0f;

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

// Everything needed to seat the partner's real entry in a slot.
struct ContestEntry {
    bool     valid = false;
    int32_t  monsNo = 0;
    uint8_t  formNo = 0;
    uint8_t  monsSex = 0;
    uint8_t  isRare = 0;
    uint8_t  rareType = 0;
    int32_t  wazaNo = 0;
    uint16_t itemNo = 0;
    uint16_t fashion = 0;      // CONTEST costume (boutique pick), not overworld
    uint8_t  playerSex = 0;
    int32_t  colorID = 0;
    uint8_t  conditions[6] = {}; // style, beautiful, cute, clever, strong, fur
    float    wazaSeqTime = 0.0f;
    uint8_t  ballId = 0;
    uint8_t  userLangID = 0;
    uint8_t  monsLangID = 0;
    uint8_t  category = 0;
    uint8_t  rank = 0;
    char     nickname[16] = {};
    char     playerName[16] = {};
};

struct ContestScores {
    bool    valid = false;
    int32_t visual = 0;
    int32_t dance = 0;
    int32_t waza = 0;
};

struct ContestState {
    bool    armed = false;
    int32_t partner = -1;

    ContestEntry  myEntry;
    ContestEntry  peerEntry;
    ContestScores mine;
    ContestScores peer;

    // Entrant slot the partner occupies on OUR stage (-1 = not installed).
    int32_t peerSlot = -1;

    // EvCmdContestProc stall bookkeeping
    int32_t evWaitCalls = 0;
    bool    evWaitToastShown = false;

    // Deferred Dance->Result boundary
    bool    pendingBoundary = false;
    void*   pendingController = nullptr;
    float   waitTimer = 0.0f;

    void ClearRound() {
        myEntry = ContestEntry{};
        peerEntry = ContestEntry{};
        mine = ContestScores{};
        peer = ContestScores{};
        peerSlot = -1;
        evWaitCalls = 0;
        evWaitToastShown = false;
        pendingBoundary = false;
        pendingController = nullptr;
        waitTimer = 0.0f;
    }
};
static ContestState s_ct;

bool mpContestIsArmed() { return s_ct.armed; }

// Contest entry itself is gated by vanilla (reception refuses before the story
// unlock), so the counter entry stays available whenever teamed up.
bool mpContestUnlocked() { return true; }

void mpContestArm(int32_t partnerStation) {
    s_ct.armed = true;
    s_ct.partner = partnerStation;
    s_ct.ClearRound();
    MP_LOG("[Contest] Co-op armed with station %d\n", partnerStation);
}

void mpContestDisarm() {
    if (!s_ct.armed) return;
    MP_LOG("[Contest] Co-op disarmed\n");
    s_ct.armed = false;
    s_ct.partner = -1;
    s_ct.ClearRound();
}

void mpContestOnPeerLeft(int32_t stationIndex) {
    if (s_ct.armed && stationIndex == s_ct.partner) {
        mpContestDisarm();
    }
}

// ---------------------------------------------------------------------------
// Local entry capture (from ContestWork.entryPlayerDatas[0])
// ---------------------------------------------------------------------------

static bool contestCaptureLocalEntry() {
    auto* statics = Dpr::Contest::ContestWork::getClass()->static_fields;
    if (statics == nullptr || statics->entryPlayerDatas == nullptr) return false;
    auto* arr = statics->entryPlayerDatas;
    if (arr->max_length < 1 || arr->m_Items[0] == nullptr) return false;
    auto& rec = arr->m_Items[0]->fields;
    if (rec.monsNo <= 0) return false;

    ContestEntry& e = s_ct.myEntry;
    e = ContestEntry{};
    e.monsNo      = rec.monsNo;
    e.formNo      = (uint8_t)rec.formNo;
    e.monsSex     = rec.monsSex;
    e.isRare      = rec.isRare ? 1 : 0;
    e.rareType    = rec.rareType;
    e.wazaNo      = rec.wazaNo;
    e.itemNo      = rec.itemNo;
    e.fashion     = rec.fashion;
    e.playerSex   = rec.playerSex;
    e.colorID     = rec.colorID;
    e.conditions[0] = rec.style;
    e.conditions[1] = rec.beautiful;
    e.conditions[2] = rec.cute;
    e.conditions[3] = rec.clever;
    e.conditions[4] = rec.strong;
    e.conditions[5] = rec.fur;
    e.wazaSeqTime = rec.wazaSeqTime;
    e.ballId      = rec.ballInfo.fields.ballId;
    e.userLangID  = (uint8_t)rec.userLangID;
    e.monsLangID  = (uint8_t)rec.monsLangID;
    e.category    = (uint8_t)statics->categoryID;
    e.rank        = (uint8_t)statics->rankID;

    if (rec.pokemonNickName != nullptr) {
        auto nick = rec.pokemonNickName->asCString();
        strncpy(e.nickname, nick.c_str(), sizeof(e.nickname) - 1);
    }
    if (rec.playerName != nullptr) {
        auto name = rec.playerName->asCString();
        strncpy(e.playerName, name.c_str(), sizeof(e.playerName) - 1);
    }

    e.valid = true;
    MP_LOG("[Contest] Local entry: mons=%d form=%d costume=%d cat=%d rank=%d '%s'\n",
                e.monsNo, (int)e.formNo, (int)e.fashion, (int)e.category, (int)e.rank,
                e.nickname);
    return true;
}

// ---------------------------------------------------------------------------
// Packets
// ---------------------------------------------------------------------------

// 0xD9 CONTEST_ENTRY (targeted): everything to seat our entry on their stage.
static void contestSendEntry() {
    if (!s_ct.armed || !s_ct.myEntry.valid) return;
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    const ContestEntry& e = s_ct.myEntry;
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_CONTEST_ENTRY);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_ct.partner);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, e.monsNo);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.formNo);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.monsSex);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.isRare);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.rareType);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, e.wazaNo);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, (int32_t)e.itemNo);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, (int32_t)e.fashion);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.playerSex);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, e.colorID);
    for (int i = 0; i < 6; i++)
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.conditions[i]);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, e.wazaSeqTime);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.ballId);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.userLangID);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.monsLangID);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.category);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.rank);

    uint8_t nickLen = (uint8_t)strnlen(e.nickname, sizeof(e.nickname) - 1);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, nickLen);
    for (uint8_t i = 0; i < nickLen; i++)
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)e.nickname[i]);
    uint8_t nameLen = (uint8_t)strnlen(e.playerName, sizeof(e.playerName) - 1);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, nameLen);
    for (uint8_t i = 0; i < nameLen; i++)
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)e.playerName[i]);

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[Contest] Sent entry (mons=%d costume=%d cat=%d rank=%d)\n",
                e.monsNo, (int)e.fashion, (int)e.category, (int)e.rank);
}

void mpContestOnEntryReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);

    ContestEntry e = {};
    int32_t itemNo = 0, fashion = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &e.monsNo);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.formNo);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.monsSex);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.isRare);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.rareType);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &e.wazaNo);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &itemNo);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &fashion);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.playerSex);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &e.colorID);
    for (int i = 0; i < 6; i++)
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.conditions[i]);
    il2cpp_vcall_read_out(pr, PR_READ_FP32_OUT, &e.wazaSeqTime);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.ballId);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.userLangID);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.monsLangID);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.category);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.rank);
    e.itemNo = (uint16_t)itemNo;
    e.fashion = (uint16_t)fashion;

    uint8_t nickLen = 0;
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &nickLen);
    if (nickLen >= sizeof(e.nickname)) return; // malformed
    for (uint8_t i = 0; i < nickLen; i++)
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, (uint8_t*)&e.nickname[i]);
    uint8_t nameLen = 0;
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &nameLen);
    if (nameLen >= sizeof(e.playerName)) return; // malformed
    for (uint8_t i = 0; i < nameLen; i++)
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, (uint8_t*)&e.playerName[i]);

    if (target != mpThisStationIndex()) return;
    if (!s_ct.armed || fromStation != s_ct.partner) return;

    // Distrust posture: plausibility bounds on peer-supplied data.
    if (e.monsNo <= 0 || e.monsNo > 905 || e.formNo > 63 ||
        e.wazaNo < 0 || e.wazaNo > 900 ||
        e.category < 1 || e.category > 7 || e.rank < 1 || e.rank > 4) {
        MP_LOG("[Contest] Rejected implausible entry (mons=%d waza=%d cat=%d rank=%d)\n",
                    e.monsNo, e.wazaNo, (int)e.category, (int)e.rank);
        return;
    }

    e.valid = true;
    s_ct.peerEntry = e;
    MP_LOG("[Contest] Partner entry: mons=%d form=%d costume=%d cat=%d rank=%d '%s' by '%s'\n",
                e.monsNo, (int)e.formNo, (int)e.fashion, (int)e.category, (int)e.rank,
                e.nickname, e.playerName);
}

// 0xD8 CONTEST_RESULT (targeted): final section scores.
static void contestSendResult() {
    if (!s_ct.armed || !s_ct.mine.valid) return;
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_CONTEST_RESULT);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_ct.partner);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_ct.mine.visual);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_ct.mine.dance);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_ct.mine.waza);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[Contest] Sent scores: v=%d d=%d w=%d\n",
                s_ct.mine.visual, s_ct.mine.dance, s_ct.mine.waza);
}

// Forward decl: resume a deferred section boundary (needs the hook's Orig).
static void contestResumeBoundary();

void mpContestOnResultReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0, visual = 0, dance = 0, waza = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &visual);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &dance);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &waza);

    if (target != mpThisStationIndex()) return;
    if (!s_ct.armed || fromStation != s_ct.partner) return;

    if (visual < 0 || visual > 10000 || dance < 0 || dance > 10000 ||
        waza < 0 || waza > 10000) {
        MP_LOG("[Contest] Rejected implausible partner scores\n");
        return;
    }

    s_ct.peer.valid = true;
    s_ct.peer.visual = visual;
    s_ct.peer.dance = dance;
    s_ct.peer.waza = waza;
    MP_LOG("[Contest] Partner scores: v=%d d=%d w=%d\n", visual, dance, waza);

    // If our contest is stalled at the boundary, release it now (this runs on
    // the main thread — the packet pump is Sequencer-driven).
    if (s_ct.pendingBoundary) {
        contestResumeBoundary();
    }
}

// ---------------------------------------------------------------------------
// Score read/inject on the contest entities
// ---------------------------------------------------------------------------
// controller->objectManagerPtr(+0x40) -> entity array(+0x38) -> entity[slot]
// (+0x20 + slot*8); entity+0x60 = AContestPlayerData:
//   +0x40 = PlayerVisualDataModel (heartNum @ +0x10)
//   +0x48 = PlayerDanceDataModel  (skillScore @ +0x28, danceScore @ +0x2C)

static bool contestSlotModels(void* controller, int slot, void** visualOut, void** danceOut) {
    void* om = *(void**)((uintptr_t)controller + CC_OBJECT_MANAGER);
    if (om == nullptr) return false;
    void* arr = *(void**)((uintptr_t)om + 0x38);
    if (arr == nullptr) return false;
    uint32_t len = *(uint32_t*)((uintptr_t)arr + 0x18);
    if ((uint32_t)slot >= len) return false;
    void* entity = *(void**)((uintptr_t)arr + 0x20 + (uint64_t)slot * 8);
    if (entity == nullptr) return false;
    void* playerData = *(void**)((uintptr_t)entity + 0x60);
    if (playerData == nullptr) return false;
    void* vm = *(void**)((uintptr_t)playerData + 0x40);
    void* dm = *(void**)((uintptr_t)playerData + 0x48);
    if (vm == nullptr || dm == nullptr) return false;
    *visualOut = vm;
    *danceOut = dm;
    return true;
}

static bool contestCaptureLocalScores(void* controller) {
    void* vm = nullptr; void* dm = nullptr;
    if (!contestSlotModels(controller, 0, &vm, &dm)) return false;
    s_ct.mine.visual = *(int32_t*)((uintptr_t)vm + 0x10);
    s_ct.mine.dance  = *(int32_t*)((uintptr_t)dm + 0x2C);
    s_ct.mine.waza   = *(int32_t*)((uintptr_t)dm + 0x28);
    s_ct.mine.valid = true;
    MP_LOG("[Contest] Captured local scores: v=%d d=%d w=%d\n",
                s_ct.mine.visual, s_ct.mine.dance, s_ct.mine.waza);
    return true;
}

static void contestInjectPeerScores(void* controller) {
    if (!s_ct.peer.valid || s_ct.peerSlot < 0) return;
    void* vm = nullptr; void* dm = nullptr;
    if (!contestSlotModels(controller, s_ct.peerSlot, &vm, &dm)) {
        MP_LOG("[Contest] Inject failed: no models for slot %d\n", s_ct.peerSlot);
        return;
    }
    *(int32_t*)((uintptr_t)vm + 0x10) = s_ct.peer.visual;
    *(int32_t*)((uintptr_t)dm + 0x2C) = s_ct.peer.dance;
    *(int32_t*)((uintptr_t)dm + 0x28) = s_ct.peer.waza;
    MP_LOG("[Contest] Injected partner scores into slot %d: v=%d d=%d w=%d\n",
                s_ct.peerSlot, s_ct.peer.visual, s_ct.peer.dance, s_ct.peer.waza);
}

// ---------------------------------------------------------------------------
// Hook: EvCmdContestProc — entry capture/send + wait for the partner's entry
// ---------------------------------------------------------------------------
// The reception's _CONTEST_PROC ev command commits the entry (single mode:
// ApplyUserDataToEntryPlayerData(0)) and launches the scene. We build the
// record early ourselves, send it, and return "not done" so the script re-runs
// us each frame until the partner's entry arrives — this keeps the two stages
// identical even when the players register at different speeds.
HOOK_DEFINE_TRAMPOLINE(EvCmd$$ContestProc) {
    static uint64_t Callback(void* __this) {
        if (s_ct.armed) {
            auto* statics = Dpr::Contest::ContestWork::getClass()->static_fields;
            bool tutorial = (statics != nullptr && statics->categoryID == 6);
            auto& ctx = getOverworldMPContext();
            bool partnerActive = s_ct.partner >= 0 &&
                                 ctx.remotePlayers[s_ct.partner].isActive;

            if (!tutorial && partnerActive) {
                if (!s_ct.myEntry.valid) {
                    // Build slot 0 from save data now (Orig would do the same),
                    // so we can capture + send before the scene loads.
                    Dpr::Contest::ContestWork::ApplyUserDataToEntryPlayerData(0);
                    if (contestCaptureLocalEntry()) {
                        contestSendEntry();
                        s_ct.evWaitCalls = 0;
                        s_ct.evWaitToastShown = false;
                    }
                }
                if (s_ct.myEntry.valid && !s_ct.peerEntry.valid) {
                    if (!s_ct.evWaitToastShown) {
                        s_ct.evWaitToastShown = true;
                        overworldMPShowAreaText("Waiting for your friend to register...");
                    }
                    if (++s_ct.evWaitCalls < CONTEST_ENTRY_WAIT_CALLS) {
                        return 0; // not done — re-run next frame, script waits
                    }
                    MP_LOG("[Contest] Entry wait timed out — starting solo\n");
                    overworldMPShowAreaText("Your friend didn't register — solo show.");
                } else if (s_ct.myEntry.valid && s_ct.peerEntry.valid &&
                           s_ct.evWaitToastShown) {
                    overworldMPResetAreaText();
                }
            }
        }
        return Orig(__this);
    }
};

// ---------------------------------------------------------------------------
// Hook: ContestDataModel.CreateNPCPlayerData — partner into entrant slot 1
// ---------------------------------------------------------------------------
// Single mode builds slots 1-3 as NPCs. For slot 1, overwrite the record with
// the partner's entry and build it through vanilla's own remote-human builder
// (CreateOtherPlayerData): trainer model path from THEIR contest costume,
// trainer type from THEIR sex, Pokemon fabricated from THEIR entry. The record
// keeps playerType=1 so the Dance section animates them with NPC AI (no live
// input packets needed); their REAL scores arrive at the section boundary.
HOOK_DEFINE_TRAMPOLINE(ContestDataModel$$CreateNPCPlayerData) {
    static void* Callback(void* __this, int32_t playerIndex, void* recordRaw) {
        if (s_ct.armed && s_ct.peerEntry.valid && playerIndex == 1 &&
            recordRaw != nullptr) {
            auto* statics = Dpr::Contest::ContestWork::getClass()->static_fields;
            const ContestEntry& e = s_ct.peerEntry;

            // Same-contest guard: category+rank must match or the shows are
            // different contests — fall back to a plain NPC.
            if (statics == nullptr ||
                statics->categoryID != (int32_t)e.category ||
                statics->rankID != (int32_t)e.rank) {
                MP_LOG("[Contest] Category/rank mismatch (ours %d/%d, theirs %d/%d) — NPC slot\n",
                            statics ? statics->categoryID : -1,
                            statics ? statics->rankID : -1,
                            (int)e.category, (int)e.rank);
                overworldMPShowAreaText("Your friend entered a different contest!");
                return Orig(__this, playerIndex, recordRaw);
            }

            auto& rec = ((Dpr::Contest::EntryPlayerData::Object*)recordRaw)->fields;
            rec.playerName      = System::String::Create(e.playerName);
            rec.pokemonNickName = System::String::Create(e.nickname);
            rec.fashion   = e.fashion;
            rec.wazaNo    = e.wazaNo;
            rec.monsNo    = e.monsNo;
            rec.playerSex = e.playerSex;
            rec.monsSex   = e.monsSex;
            rec.rareType  = e.rareType;
            rec.itemNo    = e.itemNo;
            rec.userLangID = (int32_t)e.userLangID;
            rec.monsLangID = (int32_t)e.monsLangID;
            rec.wazaSeqTime = e.wazaSeqTime;
            rec.formNo    = (uint32_t)e.formNo;
            // Custom palettes (-1) have no preset on the stage model — clamp
            // to 0 for now (custom-color stage models are a follow-up).
            rec.colorID   = (e.colorID >= 0) ? e.colorID : 0;
            rec.style     = e.conditions[0];
            rec.beautiful = e.conditions[1];
            rec.cute      = e.conditions[2];
            rec.clever    = e.conditions[3];
            rec.strong    = e.conditions[4];
            rec.fur       = e.conditions[5];
            rec.isRare    = e.isRare != 0;
            // Ball: id only; no seal sync in v1 (visual score is corrected to
            // the partner's REAL score at the boundary anyway).
            rec.ballInfo.fields.ballId = e.ballId;
            rec.ballInfo.fields.hasCapsule = false;
            rec.ballInfo.fields.sealCnt = 0;
            // Keep playerType=1 (NPC AI dance) and the lottery's valid
            // npcDataIndex (CreateOtherPlayerData bounds-checks it).

            void* model = _ILExternal::external<void*>(
                RVA_CREATE_OTHER_PLAYER_DATA, __this, playerIndex, recordRaw);
            if (model != nullptr) {
                s_ct.peerSlot = 1;
                MP_LOG("[Contest] Partner installed in slot 1: mons=%d costume=%d '%s'\n",
                            e.monsNo, (int)e.fashion, e.playerName);
                return model;
            }
            MP_LOG("[Contest] CreateOtherPlayerData failed — NPC fallback\n");
        }
        return Orig(__this, playerIndex, recordRaw);
    }
};

// ---------------------------------------------------------------------------
// Hook: ContestController.RequestChangeSectionId — score merge at Dance end
// ---------------------------------------------------------------------------
HOOK_DEFINE_TRAMPOLINE(ContestController$$RequestChangeSectionId) {
    static void Callback(void* __this, int32_t newSection) {
        // End-of-contest cleanup (runs regardless of merge state).
        if (newSection == SECTION_END || newSection == SECTION_RESTART) {
            if (s_ct.armed) {
                s_ct.mine = ContestScores{};
                s_ct.peer = ContestScores{};
                s_ct.pendingBoundary = false;
                s_ct.pendingController = nullptr;
                if (newSection == SECTION_END) {
                    // Entries are per-show; the pairing stays armed for the
                    // next contest (until disband).
                    s_ct.myEntry = ContestEntry{};
                    s_ct.peerEntry = ContestEntry{};
                    s_ct.peerSlot = -1;
                }
            }
            Orig(__this, newSection);
            return;
        }

        if (s_ct.armed && s_ct.peerSlot >= 0 &&
            newSection == SECTION_PRELOAD_RESULT &&
            *(int32_t*)((uintptr_t)__this + CC_CURRENT_SECTION) == SECTION_DANCE) {
            // Dance just ended — the show's scores are final on our side.
            if (!s_ct.mine.valid) {
                if (contestCaptureLocalScores(__this)) {
                    contestSendResult();
                }
            }
            if (!s_ct.peer.valid) {
                // Stall the transition until the partner's scores arrive.
                // The section keeps re-requesting (or the 0xD8 receipt calls
                // contestResumeBoundary), so nothing is lost by swallowing.
                if (!s_ct.pendingBoundary) {
                    s_ct.pendingBoundary = true;
                    s_ct.pendingController = __this;
                    s_ct.waitTimer = CONTEST_BOUNDARY_TIMEOUT_SEC;
                    MP_LOG("[Contest] Holding at Dance end — waiting for partner scores\n");
                }
                return;
            }
            contestInjectPeerScores(__this);
            s_ct.pendingBoundary = false;
            s_ct.pendingController = nullptr;
        }
        Orig(__this, newSection);
    }
};

static void contestResumeBoundary() {
    if (!s_ct.pendingBoundary || s_ct.pendingController == nullptr) return;
    void* ctrl = s_ct.pendingController;
    s_ct.pendingBoundary = false;
    s_ct.pendingController = nullptr;
    if (s_ct.peer.valid) {
        contestInjectPeerScores(ctrl);
    } else {
        MP_LOG("[Contest] Boundary released without partner scores\n");
    }
    ContestController$$RequestChangeSectionId::Orig(ctrl, SECTION_PRELOAD_RESULT);
    MP_LOG("[Contest] Boundary released -> PreLoadResult\n");
}

// ---------------------------------------------------------------------------
// Tick (safety timeout; primary resume is 0xD8 receipt)
// ---------------------------------------------------------------------------
void mpContestTick(float deltaTime) {
    if (!s_ct.pendingBoundary) return;

    auto& ctx = getOverworldMPContext();
    if (s_ct.partner < 0 || !ctx.remotePlayers[s_ct.partner].isActive) {
        contestResumeBoundary();
        return;
    }

    s_ct.waitTimer -= deltaTime;
    if (s_ct.waitTimer <= 0.0f) {
        MP_LOG("[Contest] Boundary timeout — proceeding without partner scores\n");
        contestResumeBoundary();
    }
}

void exl_mp_contest_hooks() {
    EvCmd$$ContestProc::InstallAtOffset(RVA_EVCMD_CONTEST_PROC);
    ContestDataModel$$CreateNPCPlayerData::InstallAtOffset(RVA_CREATE_NPC_PLAYER_DATA);
    ContestController$$RequestChangeSectionId::InstallAtOffset(RVA_REQUEST_CHANGE_SECTION);
    MP_LOG("[Contest] Scene hooks installed (entry, slot-1 install, score boundary)\n");
}
