#include "exlaunch.hpp"
#include "features/mp_log.h"
#include "features/mp_net.h"
#include "features/mp_minigames.h"
#include "features/overworld_multiplayer.h"

#include "helpers/InputHelper.h"

#include "externals/BaseEntity.h"
#include "externals/EntityManager.h"
#include "externals/FieldCanvas.h"
#include "externals/GameData/DataManager.h"
#include "externals/GameManager.h"
#include "externals/ItemWork.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokeParty.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Vector3.h"

#include <cstdio>
#include <cstring>
#include <cmath>

// ---------------------------------------------------------------------------
// Tuning
// ---------------------------------------------------------------------------
static constexpr float HNS_HIDE_SECONDS    = 30.0f;
static constexpr float HNS_SEEK_SECONDS    = 180.0f;
static constexpr float HNS_FOUND_RADIUS    = 2.5f;   // slightly over interact radius
static constexpr float HNS_PING_INTERVAL   = 10.0f;  // warmer/colder cadence
static constexpr float CATCH_SECONDS       = 300.0f; // 5-minute catching window
static constexpr float HUD_TICK_INTERVAL   = 30.0f;  // periodic time-left reminder

// EVENT_END reasons
enum class EndReason : uint8_t {
    Found   = 0,  // seeker found the hider (hide-and-seek)
    Timeout = 1,  // timer expired (hider wins HNS; catch contest → results)
    Forfeit = 2,  // zone change / disconnect / manual abort
};

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------
struct MinigameState {
    MinigameKind kind = MinigameKind::None;
    int32_t partner = -1;
    bool iAmInitiator = false;

    float timer = 0.0f;       // seconds remaining in the current phase
    uint8_t phase = 0;        // HNS: 0=hide 1=seek; CATCH: 0=running 1=awaiting result
    float hudTimer = 0.0f;
    float pingTimer = 0.0f;
    float lastDistSq = -1.0f;

    // Catching contest
    uint32_t partySnapshot[6] = {};
    int32_t snapshotCount = 0;
    int32_t contestZone = 0;
    bool myResultSent = false;
    bool peerResultReceived = false;
    int32_t myScore = 0;
    int32_t peerScore = 0;
    uint16_t myMons = 0, peerMons = 0;
    uint8_t myLevel = 0, peerLevel = 0;

    void Clear() { *this = MinigameState{}; }
};
static MinigameState s_mg;

bool mpMinigameIsActive() { return s_mg.kind != MinigameKind::None; }

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static void hudMsg(const char* text) {
    FieldCanvas::Debug_ShowDisplayMessage(System::String::Create(text));
}

// Label-routed HUD message: uses the ss_multiplayer bundle label when it
// exists, the English fallback otherwise. Labels ship via the companion
// Romhack_Unity message-bundle PR.
static void hudMsgL(const char* label, const char* fallback) {
    hudMsg(overworldMPGetMessageCStr(label, fallback));
}

static void hudMsgF(const char* fmt, int v) {
    char buf[96];
    snprintf(buf, sizeof(buf), fmt, v);
    hudMsg(buf);
}

static bool getLocalPlayerPos(UnityEngine::Vector3::Object* out) {
    EntityManager::getClass()->initIfNeeded();
    auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    if (player == nullptr) return false;
    *out = player->cast<BaseEntity>()->fields.worldPosition;
    return true;
}

static float distSqToPartner() {
    UnityEngine::Vector3::Object me;
    if (!getLocalPlayerPos(&me)) return -1.0f;
    auto& ctx = getOverworldMPContext();
    if (s_mg.partner < 0 || !ctx.remotePlayers[s_mg.partner].isActive) return -1.0f;
    auto& rp = ctx.remotePlayers[s_mg.partner].position;
    float dx = rp.fields.x - me.fields.x;
    float dz = rp.fields.z - me.fields.z;
    return dx * dx + dz * dz;
}

static void sendEnd(EndReason reason) {
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_EVENT_END);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_mg.partner);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)reason);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
}

static void sendMyResult() {
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_EVENT_RESULT);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_mg.partner);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_mg.myScore);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, (int32_t)s_mg.myMons);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, s_mg.myLevel);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    s_mg.myResultSent = true;
    MP_LOG("[Minigame] Sent catch result: score=%d mons=%u lv=%u\n",
                s_mg.myScore, s_mg.myMons, s_mg.myLevel);
}

// Restore any hidden partner entity and drop the HUD.
static void endLocal(const char* resultText) {
    if (s_mg.kind == MinigameKind::HideAndSeek && s_mg.partner >= 0) {
        overworldMPSetEntityVisible(s_mg.partner, true);
    }
    if (resultText != nullptr) hudMsg(resultText);
    MP_LOG("[Minigame] Ended (%s)\n", resultText ? resultText : "silent");
    s_mg.Clear();
}

// ---------------------------------------------------------------------------
// Catching contest scoring
// ---------------------------------------------------------------------------

static void snapshotParty() {
    s_mg.snapshotCount = 0;
    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;
    int32_t count = party->fields.m_memberCount;
    if (count > 6) count = 6;
    for (int32_t i = 0; i < count; i++) {
        auto* pp = party->GetMemberPointer(i);
        if (pp == nullptr) continue;
        s_mg.partySnapshot[s_mg.snapshotCount++] =
            pp->cast<Pml::PokePara::CoreParam>()->GetPersonalRnd();
    }
}

static bool wasInSnapshot(uint32_t rnd) {
    for (int32_t i = 0; i < s_mg.snapshotCount; i++)
        if (s_mg.partySnapshot[i] == rnd) return true;
    return false;
}

// Catch quality score (user design 2026-07-04): IV total (0-186)
// + level/3 rounded down (0-33) + 20 for a shiny. Max 239.
static constexpr int32_t CATCH_SCORE_MAX = 186 + 33 + 20;

static int32_t scorePokemon(Pml::PokePara::CoreParam* core) {
    int32_t ivSum = 0;
    for (int32_t t = 0; t < 6; t++)
        ivSum += (int32_t)core->GetTalentPower(t);
    return ivSum + (int32_t)core->GetLevel() / 3 + (core->IsRare() ? 20 : 0);
}

// Best NEW party member since the snapshot (caught during the contest).
static void computeMyCatchResult() {
    s_mg.myScore = 0;
    s_mg.myMons = 0;
    s_mg.myLevel = 0;
    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;
    int32_t count = party->fields.m_memberCount;
    if (count > 6) count = 6;
    for (int32_t i = 0; i < count; i++) {
        auto* pp = party->GetMemberPointer(i);
        if (pp == nullptr) continue;
        auto* core = pp->cast<Pml::PokePara::CoreParam>();
        if (wasInSnapshot(core->GetPersonalRnd())) continue;
        int32_t sc = scorePokemon(core);
        if (sc > s_mg.myScore) {
            s_mg.myScore = sc;
            s_mg.myMons = (uint16_t)core->GetMonsNo();
            s_mg.myLevel = (uint8_t)core->GetLevel();
        }
    }
}

// Prize: one random item from the Pickup ability's loot table
// (XLSXContent.MonohiroiTable: rows of {ItemNo, Ratios[10]}). The winner's
// SCORE picks the table band — a better catch rolls on a richer band, the
// same way higher-level Pickup mons find better loot.
static void grantPickupPrize(int32_t score) {
    GameData::DataManager::getClass()->initIfNeeded();
    auto* table = GameData::DataManager::getClass()->static_fields->MonohiroiTable;
    if (table == nullptr || table->fields.MonoHiroi == nullptr) {
        MP_LOG("[Minigame] MonohiroiTable unavailable — no prize\n");
        return;
    }

    // Bands 0..8 climb at the original /19 pace; the top band is reserved
    // for scores of 180+ (near-perfect IVs, or great IVs + level/shiny).
    int32_t band;
    if (score >= 180) {
        band = 9;
    } else {
        band = score / 19;
        if (band < 0) band = 0;
        if (band > 8) band = 8;
    }

    // RandomGroupWork.Value @0x199F420 — the game's general random
    int32_t roll = (int32_t)(_ILExternal::external<uint32_t>(0x199F420) % 100);

    auto* rows = table->fields.MonoHiroi;
    int32_t cumulative = 0;
    for (uint64_t i = 0; i < rows->max_length; i++) {
        auto* row = rows->m_Items[i];
        if (row == nullptr || row->fields.Ratios == nullptr) continue;
        if ((uint64_t)band >= row->fields.Ratios->max_length) continue;
        cumulative += (int32_t)row->fields.Ratios->m_Items[band];
        if (roll < cumulative) {
            int32_t itemNo = (int32_t)row->fields.ID;
            ItemWork::AddItem(itemNo, 1);
            MP_LOG("[Minigame] Contest prize: item %d (band %d, roll %d)\n",
                        itemNo, band, roll);
            hudMsgL("SS_mp_Catch_Prize", "You won a prize! Check your bag!");
            return;
        }
    }
    MP_LOG("[Minigame] Prize roll missed the table (band %d, roll %d)\n", band, roll);
}

// Peer result must be a species/level that can exist in the contest zone.
static bool validatePeerResult() {
    if (s_mg.peerScore == 0) return true;  // "caught nothing" is always valid
    if (s_mg.peerScore < 0 || s_mg.peerScore > CATCH_SCORE_MAX) return false;
    auto* table = GameManager::GetFieldEncountData(s_mg.contestZone);
    if (table == nullptr) return false;
    MonsLv::Array* lists[] = {
        table->fields.ground_mons, table->fields.tairyo,
        table->fields.day, table->fields.night, table->fields.swayGrass,
        table->fields.water_mons, table->fields.boro_mons,
        table->fields.ii_mons, table->fields.sugoi_mons,
    };
    for (auto* slots : lists) {
        if (slots == nullptr) continue;
        for (uint64_t i = 0; i < slots->max_length; i++) {
            auto& slot = slots->m_Items[i];
            if (slot.fields.monsNo == (int32_t)s_mg.peerMons &&
                (int32_t)s_mg.peerLevel >= slot.fields.minlv - 5 &&
                (int32_t)s_mg.peerLevel <= slot.fields.maxlv + 5) {
                return true;
            }
        }
    }
    return false;
}

static void showCatchWinner() {
    bool peerOk = validatePeerResult();
    if (!peerOk) {
        MP_LOG("[Minigame] Peer result failed zone validation (mons=%u lv=%u zone=%d)\n",
                    s_mg.peerMons, s_mg.peerLevel, s_mg.contestZone);
        s_mg.peerScore = 0;
    }
    char buf[128];
    int32_t myScore = s_mg.myScore;
    bool iWin = myScore > s_mg.peerScore;
    bool tie = myScore == s_mg.peerScore && myScore > 0;
    if (iWin) {
        snprintf(buf, sizeof(buf), "You win the Catching Contest! (%d vs %d)",
                 s_mg.myScore, s_mg.peerScore);
    } else if (tie) {
        snprintf(buf, sizeof(buf), "Catching Contest tied at %d!", s_mg.myScore);
    } else if (myScore < s_mg.peerScore) {
        snprintf(buf, sizeof(buf), "Your friend wins the Catching Contest! (%d vs %d)",
                 s_mg.peerScore, s_mg.myScore);
    } else {
        snprintf(buf, sizeof(buf), "No catches — no contest!");
    }
    endLocal(buf);

    // Winner takes a Pickup-table prize on their own console (ties: both).
    if (iWin || tie) {
        grantPickupPrize(myScore);
    }
}

// ---------------------------------------------------------------------------
// Start / receive
// ---------------------------------------------------------------------------

static void startLocal(MinigameKind kind, int32_t partner, bool initiator) {
    s_mg.Clear();
    s_mg.kind = kind;
    s_mg.partner = partner;
    s_mg.iAmInitiator = initiator;
    s_mg.hudTimer = 0.0f;

    if (kind == MinigameKind::HideAndSeek) {
        s_mg.phase = 0;
        s_mg.timer = HNS_HIDE_SECONDS;
        // Initiator hides. The seeker's console blinds the hider's entity now.
        bool iAmSeeker = !initiator;
        if (iAmSeeker) {
            overworldMPSetEntityVisible(partner, false);
            hudMsgL("SS_mp_HnS_TheyHide", "Hide-and-Seek! Your friend is hiding... (30s)");
        } else {
            hudMsgL("SS_mp_HnS_GoHide", "Hide-and-Seek! Go hide! (30s)");
        }
    } else if (kind == MinigameKind::CatchContest) {
        s_mg.phase = 0;
        s_mg.timer = CATCH_SECONDS;
        s_mg.contestZone = PlayerWork::get_zoneID();
        snapshotParty();
        hudMsgL("SS_mp_Catch_Start", "Catching Contest! Best new catch in 5 minutes wins!");
    }
    MP_LOG("[Minigame] Started kind=%d partner=%d initiator=%d\n",
                (int)kind, partner, (int)initiator);
}

void mpMinigameStartAsInitiator(int32_t partnerStation, MinigameKind kind) {
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_EVENT_START);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, partnerStation);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)kind);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, PlayerWork::get_zoneID());
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);

    startLocal(kind, partnerStation, true);
}

void mpMinigameOnStartReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0;
    uint8_t kind = 0;
    int32_t zone = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &kind);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &zone);
    if (target != mpThisStationIndex()) return;
    if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;
    if (kind != (uint8_t)MinigameKind::HideAndSeek &&
        kind != (uint8_t)MinigameKind::CatchContest) return;

    startLocal((MinigameKind)kind, fromStation, false);
    if ((MinigameKind)kind == MinigameKind::CatchContest) {
        s_mg.contestZone = zone;  // both validate against the initiator's zone
    }
}

void mpMinigameOnEndReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0;
    uint8_t reason = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &reason);
    if (target != mpThisStationIndex()) return;
    if (!mpMinigameIsActive() || fromStation != s_mg.partner) return;

    if ((EndReason)reason == EndReason::Forfeit) {
        // The sender gave up — receiver wins (no contest prize either way).
        endLocal(overworldMPGetMessageCStr("SS_mp_PeerForfeited", "Your friend gave up — you win!"));
        return;
    }

    if (s_mg.kind == MinigameKind::HideAndSeek) {
        if ((EndReason)reason == EndReason::Found) {
            endLocal(overworldMPGetMessageCStr(s_mg.iAmInitiator ? "SS_mp_HnS_WasFound" : "SS_mp_HnS_Found",
                     s_mg.iAmInitiator ? "You were found!" : "You found them!"));
        } else if ((EndReason)reason == EndReason::Timeout) {
            endLocal(overworldMPGetMessageCStr(s_mg.iAmInitiator ? "SS_mp_HnS_TimeUpWin" : "SS_mp_HnS_TimeUpLose",
                     s_mg.iAmInitiator ? "Time's up — you win!" : "Time's up — they win!"));
        } else {
            endLocal(overworldMPGetMessageCStr("SS_mp_HnS_Off", "Hide-and-Seek called off."));
        }
    } else {
        endLocal(overworldMPGetMessageCStr("SS_mp_Catch_Off", "Contest called off."));
    }
}

void mpMinigameOnResultReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0;
    int32_t score = 0, mons = 0;
    uint8_t level = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &score);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &mons);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &level);
    if (target != mpThisStationIndex()) return;
    if (s_mg.kind != MinigameKind::CatchContest || fromStation != s_mg.partner) return;

    s_mg.peerScore = score;
    s_mg.peerMons = (uint16_t)mons;
    s_mg.peerLevel = level;
    s_mg.peerResultReceived = true;
    if (s_mg.myResultSent) showCatchWinner();
}

// ---------------------------------------------------------------------------
// Tick
// ---------------------------------------------------------------------------

static void tickHideAndSeek(float dt) {
    s_mg.timer -= dt;

    if (s_mg.phase == 0) {
        if (s_mg.timer <= 0.0f) {
            s_mg.phase = 1;
            s_mg.timer = HNS_SEEK_SECONDS;
            s_mg.pingTimer = HNS_PING_INTERVAL;
            s_mg.lastDistSq = -1.0f;
            s_mg.iAmInitiator ? hudMsgL("SS_mp_HnS_StayHidden", "They're coming! Stay hidden! (3 min)")
                          : hudMsgL("SS_mp_HnS_SeekStart", "Go find them! (3 min)");
        }
        return;
    }

    // Seek phase. World coordinates are only comparable within the same area
    // (interiors have their own coordinate spaces), so both the FOUND check
    // and the pings gate on the pair being in the same area — being in a
    // different area simply reads as the coldest possible ping.
    auto& ctx = getOverworldMPContext();
    bool sameArea = (s_mg.partner >= 0 &&
                     ctx.remotePlayers[s_mg.partner].areaID == ctx.myAreaID);

    bool iAmHider = s_mg.iAmInitiator;
    if (iAmHider) {
        // FOUND authority: the hider's console knows its own true position.
        float d = sameArea ? distSqToPartner() : -1.0f;
        if (d >= 0.0f && d < HNS_FOUND_RADIUS * HNS_FOUND_RADIUS) {
            sendEnd(EndReason::Found);
            endLocal("You were found!");
            return;
        }
        // Hider owns the timeout too (one authority per outcome).
        if (s_mg.timer <= 0.0f) {
            sendEnd(EndReason::Timeout);
            endLocal("Time's up — you win!");
            return;
        }
    } else {
        // Seeker: warmer/colder pings from the synced positions.
        s_mg.pingTimer -= dt;
        if (s_mg.pingTimer <= 0.0f) {
            s_mg.pingTimer = HNS_PING_INTERVAL;
            if (!sameArea) {
                hudMsgL("SS_mp_HnS_OtherArea", "They're somewhere else...");
                s_mg.lastDistSq = -1.0f;
            } else {
                float d = distSqToPartner();
                if (d >= 0.0f) {
                    if (s_mg.lastDistSq >= 0.0f) {
                        if (d < s_mg.lastDistSq) hudMsgL("SS_mp_HnS_Warmer", "Warmer...");
                        else hudMsgL("SS_mp_HnS_Colder", "Colder...");
                    }
                    s_mg.lastDistSq = d;
                }
            }
        }
    }
}

static void tickCatchContest(float dt) {
    if (s_mg.phase == 0) {
        s_mg.timer -= dt;
        s_mg.hudTimer -= dt;
        if (s_mg.hudTimer <= 0.0f && s_mg.timer > 10.0f) {
            s_mg.hudTimer = HUD_TICK_INTERVAL;
            hudMsgF("Catching Contest: %d seconds left!", (int)s_mg.timer);
        }
        if (s_mg.timer <= 0.0f) {
            s_mg.phase = 1;
            computeMyCatchResult();
            sendMyResult();
            hudMsgL("SS_mp_Catch_TimeUp", "Time's up! Comparing catches...");
            s_mg.timer = 20.0f;  // result-exchange timeout
        }
        return;
    }

    // Awaiting peer result
    s_mg.timer -= dt;
    if (s_mg.peerResultReceived) {
        showCatchWinner();
    } else if (s_mg.timer <= 0.0f) {
        endLocal(overworldMPGetMessageCStr("SS_mp_Catch_Void", "No result from your friend — contest void."));
    }
}

void mpMinigameTick(float deltaTime) {
    if (!mpMinigameIsActive()) return;

    // Partner gone from the lobby entirely?
    auto& ctx = getOverworldMPContext();
    if (s_mg.partner < 0 || !ctx.remotePlayers[s_mg.partner].isActive) {
        endLocal(overworldMPGetMessageCStr("SS_mp_Disconnected", "Your friend disconnected — game over."));
        return;
    }

    // Forfeit control: hold ZL+ZR for 1.5s to give up (roam-anywhere means
    // there's no natural way out otherwise). Forfeiting loses; the catch
    // contest pays no prize either way.
    {
        static float s_forfeitHold = 0.0f;
        nn::hid::NpadBaseState padState = InputHelper::readNpadStateDirect();
        bool held = padState.mButtons.isBitSet(nn::hid::NpadButton::ZL) &&
                    padState.mButtons.isBitSet(nn::hid::NpadButton::ZR);
        if (held) {
            float before = s_forfeitHold;
            s_forfeitHold += deltaTime;
            if (before < 0.5f && s_forfeitHold >= 0.5f) {
                hudMsgL("SS_mp_ForfeitHold", "Keep holding ZL+ZR to give up...");
            }
            if (s_forfeitHold >= 1.5f) {
                s_forfeitHold = 0.0f;
                MP_LOG("[Minigame] Forfeit (ZL+ZR held)\n");
                sendEnd(EndReason::Forfeit);
                endLocal(overworldMPGetMessageCStr("SS_mp_Forfeited", "You gave up!"));
                return;
            }
        } else {
            s_forfeitHold = 0.0f;
        }
    }

    if (s_mg.kind == MinigameKind::HideAndSeek) tickHideAndSeek(deltaTime);
    else if (s_mg.kind == MinigameKind::CatchContest) tickCatchContest(deltaTime);
}

// ---------------------------------------------------------------------------
// Forfeit triggers
// ---------------------------------------------------------------------------

void mpMinigameOnAreaChange() {
    // Roam-anywhere by design (user decision 2026-07-04): area boundaries are
    // weird to reason about mid-chase, and the warmer/colder pings make the
    // whole world playable — different-area cases read as the coldest ping.
    // No forfeit for any current game; footrace (later) may want one.
    if (mpMinigameIsActive()) {
        MP_LOG("[Minigame] Area change during minigame — continuing (roam-anywhere)\n");
    }
}

void mpMinigameOnPeerLeft(int32_t stationIndex) {
    if (!mpMinigameIsActive() || stationIndex != s_mg.partner) return;
    endLocal(overworldMPGetMessageCStr("SS_mp_Disconnected", "Your friend disconnected — game over."));
}
