#include "exlaunch.hpp"
#include "features/mp_log.h"
#include "features/mp_net.h"
#include "features/mp_contest.h"
#include "features/overworld_multiplayer.h"
#include "features/team_up.h"

#include "externals/Dpr/NetworkUtils/NetworkManager.h"

#include <cstdint>
#include <cstring>

// ---------------------------------------------------------------------------
// Multiplayer Super Contests — see mp_contest.h for the design overview.
//
// Game structures (Ghidra research 2026-07-04):
//   ContestController: currentSectionID @0x88, nextSectionID @0x8C,
//     resultDataModel @0x90, hasRequestChangeSectionID @0x98,
//     receivedScores (ReceivedPlayerResultScore[]) @0x18 (stride 0x10:
//     isReceive @+0, visual @+4, dance @+8, waza @+C),
//     objectManagerPtr @0x40 (entity array @+0x38, elements @+0x20+k*8;
//     entity+0x60 -> visual model @+0x40 / dance model @+0x48).
//   SectionID: Opening=0 Visual=1 Dance=2 PreLoadResult=3 WaitAsync=4
//     Result=5 Restart=6 NetworkError=7 End=8.
//   Score fan-in (vanilla): ApplyReceivedPlayerResultScore @0x1C63930 writes
//     visual -> visualModel+0x10 (heartNum), dance -> danceModel+0x2C,
//     waza -> danceModel+0x28.
//   RequestChangeSectionId @0x1C63720 (deferral point: Dance -> PreLoadResult).
// ---------------------------------------------------------------------------

// ContestController field offsets
static constexpr uintptr_t CC_RECEIVED_SCORES = 0x18;
static constexpr uintptr_t CC_OBJECT_MANAGER  = 0x40;
static constexpr uintptr_t CC_CURRENT_SECTION = 0x88;
static constexpr uintptr_t CC_RESULT_MODEL    = 0x90;

// SectionID values
static constexpr int32_t SECTION_OPENING        = 0;
static constexpr int32_t SECTION_VISUAL         = 1;
static constexpr int32_t SECTION_DANCE          = 2;
static constexpr int32_t SECTION_PRELOAD_RESULT = 3;
static constexpr int32_t SECTION_RESULT         = 5;

// Partner's scores wait this long at the section boundary before we assume
// they bailed and finish with the NPC's own performance.
static constexpr float CONTEST_WAIT_TIMEOUT_SEC = 45.0f;

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

// Entry data — enough to put the partner's real Pokémon on stage.
struct ContestEntry {
    bool     valid = false;
    int32_t  monsNo = 0;
    uint8_t  formNo = 0;
    uint8_t  sex = 0;
    uint8_t  rare = 0;        // shiny
    uint32_t personality = 0; // color/spinda-spots determinism
    uint8_t  category = 0;    // ContestCategory — must match ours
    uint8_t  rank = 0;        // contest rank — must match ours
    char     nickname[16] = {};   // ASCII-flattened, display only
    char     playerName[16] = {}; // partner's trainer name
};

struct ContestScores {
    bool    valid = false;
    int32_t visual = 0;
    int32_t dance = 0;
    int32_t waza = 0;  // appeal
};

struct ContestState {
    bool    armed = false;
    int32_t partner = -1;

    ContestEntry  myEntry;
    ContestEntry  peerEntry;
    ContestScores mine;
    ContestScores peer;

    // Which entrant slot (0-3) the partner's Pokémon occupies on OUR stage —
    // set when we overwrite an NPC slot with their entry; -1 = not installed.
    int32_t peerSlot = -1;

    // Deferred section boundary: controller stalled until the partner's scores.
    bool    pendingBoundary = false;
    void*   pendingController = nullptr;
    float   waitTimer = 0.0f;

    void ClearRound() {
        myEntry = ContestEntry{};
        peerEntry = ContestEntry{};
        mine = ContestScores{};
        peer = ContestScores{};
        peerSlot = -1;
        pendingBoundary = false;
        pendingController = nullptr;
        waitTimer = 0.0f;
    }
};
static ContestState s_ct;

bool mpContestIsArmed() { return s_ct.armed; }

// Contest entry itself is gated by vanilla (reception refuses before the story
// unlock), so the menu item stays available whenever teamed up.
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
// Packets
// ---------------------------------------------------------------------------

// 0xD9 CONTEST_ENTRY (targeted): sent when our contest entry is known, so the
// partner's console can seat our real Pokémon in an entrant slot before its
// stage builds. [target:4][monsNo:4][formNo:1][sex:1][rare:1][personality:4]
// [category:1][rank:1][nickLen:1][nick bytes][nameLen:1][name bytes]
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
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.sex);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e.rare);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, (int32_t)e.personality);
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
    MP_LOG("[Contest] Sent entry: mons=%d form=%d cat=%d rank=%d\n",
                e.monsNo, (int)e.formNo, (int)e.category, (int)e.rank);
}

void mpContestOnEntryReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);

    ContestEntry e = {};
    int32_t personality = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &e.monsNo);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.formNo);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.sex);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.rare);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &personality);
    e.personality = (uint32_t)personality;
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.category);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e.rank);

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

    // Distrust posture: reject implausible species (National Dex bound).
    if (e.monsNo <= 0 || e.monsNo > 905) {
        MP_LOG("[Contest] Rejected implausible entry mons=%d\n", e.monsNo);
        return;
    }

    e.valid = true;
    s_ct.peerEntry = e;
    MP_LOG("[Contest] Partner entry: mons=%d form=%d shiny=%d cat=%d rank=%d '%s' by '%s'\n",
                e.monsNo, (int)e.formNo, (int)e.rare, (int)e.category, (int)e.rank,
                e.nickname, e.playerName);
}

// 0xD8 CONTEST_RESULT (targeted): final section scores.
// [target:4][visual:4][dance:4][waza:4][category:1][rank:1]
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
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, s_ct.myEntry.category);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, s_ct.myEntry.rank);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[Contest] Sent result: v=%d d=%d w=%d\n",
                s_ct.mine.visual, s_ct.mine.dance, s_ct.mine.waza);
}

// Forward decl: resume a deferred section boundary.
static void contestResumeBoundary();

void mpContestOnResultReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0, visual = 0, dance = 0, waza = 0;
    uint8_t category = 0, rank = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &visual);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &dance);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &waza);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &category);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &rank);

    if (target != mpThisStationIndex()) return;
    if (!s_ct.armed || fromStation != s_ct.partner) return;

    // Distrust posture: contest section scores are small non-negatives.
    if (visual < 0 || visual > 10000 || dance < 0 || dance > 10000 ||
        waza < 0 || waza > 10000) {
        MP_LOG("[Contest] Rejected implausible partner scores\n");
        return;
    }

    s_ct.peer.valid = true;
    s_ct.peer.visual = visual;
    s_ct.peer.dance = dance;
    s_ct.peer.waza = waza;
    MP_LOG("[Contest] Partner result: v=%d d=%d w=%d\n", visual, dance, waza);

    // If our contest already reached the boundary and is stalled, resume it.
    if (s_ct.pendingBoundary) {
        contestResumeBoundary();
    }
}

// ---------------------------------------------------------------------------
// Section boundary deferral (hooks installed once entrant research lands)
// ---------------------------------------------------------------------------

static void contestResumeBoundary() {
    // Filled in by the RequestChangeSectionId hook work: injects the partner's
    // scores into their entrant slot's models and releases the transition.
    s_ct.pendingBoundary = false;
    s_ct.pendingController = nullptr;
    s_ct.waitTimer = 0.0f;
}

void mpContestTick(float deltaTime) {
    if (!s_ct.pendingBoundary) return;

    // Partner gone -> release the boundary so the local player isn't stuck.
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
    // Contest scene hooks (entrant slot overwrite + section boundary deferral)
    // install here — pending the entrant-record research pass.
    MP_LOG("[Contest] Handshake/packet layer installed (scene hooks pending)\n");
}
