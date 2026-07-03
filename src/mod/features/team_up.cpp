#include "exlaunch.hpp"
#include "features/mp_log.h"
#include "features/mp_net.h"
#include "features/mp_poke_validate.h"

#include "features/team_up.h"
#include "features/overworld_multiplayer.h"

#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/Dpr/Battle/Logic/TRAINER_DATA.h"
#include "romdata/data/ColorSet.h"
#include "externals/Dpr/EncountTools.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/DPData/MYSTATUS.h"
#include "externals/FieldManager.h"
#include "externals/UnityEngine/Time.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "externals/GameManager.h"
#include "externals/MYSTATUS_COMM.h"
#include "externals/PlayerWork.h"
#include "externals/TrainerWork.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokeParty.h"
#include "externals/System/String.h"
#include "externals/Audio/AudioManager.h"
#include "externals/DPData/CONFIG.h"
#include "save/save.h"

#include "externals/Dpr/Message/MessageManager.h"
#include "externals/il2cpp-api.h"
#include "externals/FieldCanvas.h"
#include "externals/TMPro/TMP_Text.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/Dpr/MsgWindow/MsgWindowManager.h"
#include "helpers/InputHelper.h"
#include "logger/logger.h"
#include "ui/ui.h"

// ---------------------------------------------------------------------------
// Safe FieldCanvas helpers — guard against null instance during scene transitions
// ---------------------------------------------------------------------------
static bool isFieldCanvasReady() {
    FieldCanvas::getClass()->initIfNeeded();
    auto* sf = FieldCanvas::getClass()->static_fields;
    return (sf != nullptr && sf->_instance != nullptr);
}

static void safeResetAreaNameWindow() {
    if (isFieldCanvasReady()) {
        FieldCanvas::ResetAreaNameWindow();
    }
}

// ---------------------------------------------------------------------------
// Custom area-name text hook — bypass label lookup for arbitrary text
// ---------------------------------------------------------------------------
// OpenAreaNameWindow normally takes a message label (e.g. "ZONE_NAME_C04")
// and resolves it via MessageManager. Passing raw text crashes the lookup.
// This hook intercepts AreaNameWindow::SetNameText (the label→text step)
// and sets our custom text directly on the TextMeshProUGUI component.
static bool s_useCustomAreaText = false;
static char s_customAreaTextBuf[128] = {};
static int s_dummyLabelCounter = 0;
static bool s_syncMessageShown = false;
static void* s_cachedMsgWindowTMP = nullptr; // cached TextMeshProUGUI from MsgWindow chain

HOOK_DEFINE_TRAMPOLINE(AreaNameWindowSetNameText) {
    // AreaNameWindow::SetNameText(string labelName) @ 0x21209C0
    // Instance method: (this, labelName, MethodInfo*)
    static void Callback(void* __this, System::String::Object* labelName, MethodInfo* mi) {
        if (s_useCustomAreaText) {
            // nameText (TextMeshProUGUI) field in AreaNameWindow
            static constexpr uintptr_t AREANAMEWINDOW_NAMETEXT = 0x20;
            void* nameText = *(void**)((uintptr_t)__this + AREANAMEWINDOW_NAMETEXT);
            if (nameText != nullptr) {
                auto* customStr = System::String::Create(s_customAreaTextBuf);
                if (customStr != nullptr) {
                    // TMP_Text::set_text @ 0x01e94520
                    _ILExternal::external<void>(0x01e94520, nameText, customStr);
                    MP_LOG("[TeamUp] SetNameText hook: set custom text '%s'\n",
                                s_customAreaTextBuf);
                }
            }
            s_useCustomAreaText = false;
            return;
        }
        Orig(__this, labelName, mi);
    }
};

// Show custom text in the area name window (safe: bypasses label lookup)
static void showCustomAreaNameText(const char* text) {
    if (!isFieldCanvasReady()) {
        MP_LOG("[TeamUp] showCustomAreaNameText: FieldCanvas not ready\n");
        return;
    }

    strncpy(s_customAreaTextBuf, text, sizeof(s_customAreaTextBuf) - 1);
    s_customAreaTextBuf[sizeof(s_customAreaTextBuf) - 1] = '\0';
    s_useCustomAreaText = true;

    // Reset before re-opening to restart the animation/timer
    safeResetAreaNameWindow();

    // Use a unique dummy label each time so AreaNameWindow::Open doesn't
    // skip it via IsSameShowingLabel hash comparison.
    char dummyLabel[32];
    snprintf(dummyLabel, sizeof(dummyLabel), "__tu_%d", s_dummyLabelCounter++);
    FieldCanvas::OpenAreaNameWindow(System::String::Create(dummyLabel));
}

// IL2CPP vcall helpers, PW_ offsets, POKE_FULL_DATA_SIZE: see features/mp_net.h

// ---------------------------------------------------------------------------
// Global state
// ---------------------------------------------------------------------------
static TeamUpState s_teamUpState;
static bool s_expSecondPassDone = false;  // Prevents infinite EXP re-processing in second pass

// Accumulation buffers for chunked TEAMUP_BATTLE / TEAMUP_BATTLE_ACK packets
// Non-static: accessed from overworld_multiplayer.cpp packet dispatcher via extern
int32_t s_tuAccumFromStation = -1;
uint8_t s_tuAccumMemberCount = 0;
uint8_t s_tuAccumReceivedCount = 0;
uint8_t s_tuAccumBuf[6 * POKE_FULL_DATA_SIZE + 128] = {};
int32_t s_tuAccumBufSize = 0;
uint8_t s_tuAccumBattleType = 0;  // 1=trainer (only trainer team-up supported)
bool    s_tuAccumIsAck = false;    // true if accumulating ACK (not initial BATTLE)

// Trainer party accumulation (TEAMUP_SUB_TRAINER_POKE sub-packets from initiator)
uint8_t s_tuAccumTrainerCount = 0;    // expected trainer party members (from header)
uint8_t s_tuAccumTrainerReceived = 0; // how many trainer POKE sub-packets received so far
uint8_t s_tuAccumTrainer2Count = 0;   // expected second-trainer (slot 3) members
uint8_t s_tuAccumTrainer2Received = 0;// second-trainer POKE sub-packets received

// normalTrainer declared inline in team_up.h

// ---------------------------------------------------------------------------
// Battle effect helper: TR_MULTI animation + trainer-specific BGM
// ---------------------------------------------------------------------------
// TR_MULTI (68) gives us the 2v2 multi-battle intro animation, but it also
// overrides the BGM to the generic multi-battle theme. This helper sets up
// the trainer's original effect first to capture the correct BGM strings,
// then applies TR_MULTI for the animation, and restores the BGM overrides.
//
// BattleEffectComponentData layout:
//   +0x10: battleBgm override (String*, null = fall through to template)
//   +0x18: winBgm override (String*, null = fall through to template)
//   +0x20: template pointer (BattleSetupEffectData from table)
//   +0x28: effectBattleID, +0x30: soundEventName, +0x38: cmdSeq
//
// RVAs:
//   SetUpBattleEffectComponentData: 0x0187B7E0
//   get_battleBgm: 0x0187B510  (checks +0x10 first, falls through to template+0x28)
//   get_winBgm:    0x0187B570  (checks +0x18 first, falls through to template+0x30)
//   set_battleBgm: 0x0187B560  (writes +0x10 + GC barrier)
//   set_winBgm:    0x0187B5C0  (writes +0x18 + GC barrier)

static constexpr int32_t TR_MULTI = 68;

static void setupTeamUpBattleEffect(void* btlEffComponent, int32_t trainerEffectID) {
    if (btlEffComponent == nullptr) return;

    if (trainerEffectID >= 0 && trainerEffectID != TR_MULTI) {
        // 1. Set up with trainer's original effect to get correct BGM
        _ILExternal::external<void>(0x0187B7E0, btlEffComponent,
            trainerEffectID, (int32_t)-1, (int32_t)0, (uint64_t)0);

        // 2. Read BGM strings (overrides are null, falls through to template)
        auto* savedBgm    = _ILExternal::external<void*>(0x0187B510, btlEffComponent);
        auto* savedWinBgm = _ILExternal::external<void*>(0x0187B570, btlEffComponent);

        // 3. Override with TR_MULTI for 2v2 intro animation + cmdSeq
        _ILExternal::external<void>(0x0187B7E0, btlEffComponent,
            TR_MULTI, (int32_t)-1, (int32_t)0, (uint64_t)0);

        // 4. Restore trainer's BGM as overrides (get_battleBgm checks +0x10 first)
        _ILExternal::external<void>(0x0187B560, btlEffComponent, savedBgm);
        _ILExternal::external<void>(0x0187B5C0, btlEffComponent, savedWinBgm);

        MP_LOG("[TeamUp] Battle effect: TR_MULTI animation + trainer effect %d BGM\n",
                    trainerEffectID);
    } else {
        // No trainer effect available or already TR_MULTI — just use TR_MULTI
        _ILExternal::external<void>(0x0187B7E0, btlEffComponent,
            TR_MULTI, (int32_t)-1, (int32_t)0, (uint64_t)0);
        MP_LOG("[TeamUp] Battle effect: TR_MULTI (default BGM)\n");
    }
}

// normalTrainer and splitTrainerParty declared in team_up.h

// Cached BSP pointer for Player A's in-flight battle modification
// Non-static: also accessed from trainer_double_battle.cpp via extern
Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* s_teamUpBSP = nullptr;

// Guard flag for CreateLocalClient hook
static bool s_teamUpCreateLocalClientActive = false;

// Pre-Orig guard: set BEFORE SetupBattleTrainer::Orig() so DeferEncountStart
// can catch EncountStart calls that happen INSIDE SetupBattleTrainer.
// Without this, if EncountStart is called from within SetupBattleTrainer::Orig(),
// syncPhase would still be SYNC_NONE and the deferral condition would fail.
bool s_teamUpEncountPending = false;

// Saved local wazaeff_mode — comm battles sync animation settings from the
// server, overwriting the local player's preference. Save before battle, restore after.
static int32_t s_savedWazaeffMode = -1;

// ---------------------------------------------------------------------------
// Sync-wait state — manages the trainer rendezvous before team-up battles
// ---------------------------------------------------------------------------
static float s_syncMessageTimer = 0.0f;          // countdown to refresh waiting message
static constexpr float SYNC_MESSAGE_INTERVAL = 4.0f; // re-show area name every 4s (auto-fades)

// ---------------------------------------------------------------------------
// Deferred EncountStart — prevents battle engine from reading BSP before
// the ACK handler has finished modifying it. Over the internet, the ACK
// takes 100-300ms round trip, but the event script calls EncountStart on
// the same frame as SetupBattleTrainer. Without deferral, the battle
// engine consumes the unmodified trainer BSP → crashes/corruption.
// ---------------------------------------------------------------------------
static bool s_encountDeferred = false;
static FieldManager::Object* s_deferredFM = nullptr;
static int32_t s_deferredType = 0;
static int32_t s_deferredTrainer1 = 0;
static int32_t s_deferredTrainer2 = 0;
static float s_deferredTimer = 0.0f;
static constexpr float DEFERRED_ENCOUNT_TIMEOUT = 5.0f;

// ---------------------------------------------------------------------------
// Full party save/restore — team-up battles use TEAMUP_PARTY_LIMIT (3)
// Pokemon per player. The vanilla storeBattleResult copies only 3 Pokemon
// back to the player's party, destroying entries 4-6 → Bad Eggs.
// Save the full party before battle, restore non-participating Pokemon after.
// ---------------------------------------------------------------------------
static uint8_t s_savedFullPartyBuf[6 * POKE_FULL_DATA_SIZE];
static int32_t s_savedFullPartyCount = 0;

// Battle-modified party buffer — during storeBattleResult, save the battle-
// modified Pokemon (slots 0-TEAMUP_PARTY_LIMIT) before BSP::Clear wipes them.
// The actual restore happens in the post-battle handler, AFTER the vanilla
// FinalizeCoroutine completes (which otherwise overwrites our storeBattleResult restore).
static uint8_t s_battleModPartyBuf[TEAMUP_PARTY_LIMIT * POKE_FULL_DATA_SIZE];
static int32_t s_battleModPartyCount = 0;
static bool s_battleAbnormal = false;
// True when the (non-abnormal) team-up battle was a LOSS — the deferred restore
// then heals the ENTIRE party to full (the pokémon-center restore) while keeping
// the exp/level/move gains.
static bool s_battleWasLoss = false;

// Deferred exp capture — storeBattleResult fires BEFORE the final KO's exp is
// fully applied to the battle POKECON, so capturing there under-counts the last
// chunk (often a whole level). Instead we record the capture parameters here and
// perform the actual POKECON→party conversion in the BATTLE_SETUP_PARAM::Clear
// hook, which runs at the very end of FinalizeCoroutine — after every exp screen
// has resolved but before the BSP is wiped and before the evolution check reads
// PlayerWork. s_captureMainModule stays valid for that window.
static bool  s_pendingExpCapture = false;
static void* s_captureMainModule = nullptr;
static uint8_t s_captureClientId = 0;
static bool  s_captureIsWin = false;
// The src party resolved at storeBattleResult (where GetSrcParty is valid). By
// Clear time the accessor returns null, but the party OBJECT is still alive, so
// we reuse this saved pointer as the conversion destination for the late top-up.
static Pml::PokeParty::Object* s_captureSrcParty = nullptr;

// The POKECON the client-side ServerCommandExecutor actually applies exp into (via
// scProc_OP_AddExp → BTL_POKEPARAM::AddExp). On the joiner this is the LOCAL client
// pokecon that executes every synced exp command (→ the leveled-up value you see),
// which is a DIFFERENT object than the server-synced pokecon at mainModule+0x110 we
// used to read (it lags the final KO). Captured live during the battle; used as the
// conversion source at battle end. Reset per battle.
static void* s_expApplyPokecon = nullptr;

// Win money captured from our CalcWinMoney hook (which ungates it for team-up). The
// game's MainModule bonus-money field isn't populated until near the exit, so we can't
// read it at storeBattleResult — carry the value here and credit it on the win.
static int32_t s_teamUpWinMoney = 0;

// Per-enemy "our own pokemon already got this KO's exp" set, indexed [side][idx].
// The comm exp calc normally skips enemies whose "checked" flag is set — but the host
// SYNCS that flag to the joiner, so the joiner's own exp calc skips the final KO (the
// host processed it first). We drive the local (first) pass off THIS set instead, so
// each client commits every KO its own pokemon earned exactly once, regardless of the
// host's synced flag. Reset per battle.
static bool s_localAwarded[4][6] = {};

// Reset the per-battle exp-capture state at the start of each team-up battle.
// Everything here must be cleared each battle so a prior battle's state (esp. after
// an abnormal ending that skips saveBattleModifiedParty) can't leak forward.
void owmpTeamUpResetBattleResultSync() {
    memset(s_localAwarded, 0, sizeof(s_localAwarded));
    s_expApplyPokecon = nullptr;
    s_pendingExpCapture = false;   // else a leaked pending fires at an unrelated Clear
    s_battleWasLoss = false;       // else a stale loss full-heals a later battle
    s_captureSrcParty = nullptr;
    s_captureMainModule = nullptr;
}

// Second-pass restore — vanilla FinalizeCoroutine may overwrite our deferred
// restore. This timer triggers a re-apply one frame later as a safety net.
float s_postBattleRestoreTimer = 0.0f;

TeamUpState& overworldMPGetTeamUpState() {
    return s_teamUpState;
}

bool overworldMPIsTeamedUp() {
    return s_teamUpState.isTeamedUp;
}

// ---------------------------------------------------------------------------
// Full party save/restore
// ---------------------------------------------------------------------------
void overworldMPSaveFullParty() {
    // New battle → clear any stale host-result sync state (received team from a prior
    // fight, and the initiator's send-once latch).
    owmpTeamUpResetBattleResultSync();
    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;
    s_savedFullPartyCount = party->fields.m_memberCount;
    if (s_savedFullPartyCount > 6) s_savedFullPartyCount = 6;
    for (int i = 0; i < s_savedFullPartyCount; i++) {
        auto* poke = party->GetMemberPointer(i);
        if (poke == nullptr || poke->fields.m_accessor == nullptr) {
            // Truncate — never record more slots than successfully serialized
            s_savedFullPartyCount = i;
            MP_LOG("[TeamUp] WARNING: truncated save at slot %d (null accessor)\n", i);
            break;
        }
        poke->fields.m_accessor->Serialize_FullData(
            &s_savedFullPartyBuf[i * POKE_FULL_DATA_SIZE]);
    }
    MP_LOG("[TeamUp] Saved full party: %d members\n", s_savedFullPartyCount);
}

static void restoreNonParticipatingPokemon() {
    if (s_savedFullPartyCount <= TEAMUP_PARTY_LIMIT) return; // nothing beyond slot 3 to restore

    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;

    // Temporarily set member count high so GetMemberPointer can access slots 3+.
    // The battle engine reduces m_memberCount to TEAMUP_PARTY_LIMIT (3).
    int32_t prevCount = party->fields.m_memberCount;
    party->fields.m_memberCount = s_savedFullPartyCount;

    int32_t restored = 0;
    for (int i = TEAMUP_PARTY_LIMIT; i < s_savedFullPartyCount; i++) {
        auto* poke = party->GetMemberPointer(i);
        if (poke != nullptr && poke->fields.m_accessor != nullptr) {
            poke->fields.m_accessor->Deserialize_FullData(
                &s_savedFullPartyBuf[i * POKE_FULL_DATA_SIZE]);
            restored++;
        } else {
            MP_LOG("[TeamUp] WARNING: slot %d poke=%p accessor=%p — cannot restore!\n",
                        i, poke, poke ? poke->fields.m_accessor : nullptr);
        }
    }
    // Set final count = participating slots + successfully restored slots
    party->fields.m_memberCount = TEAMUP_PARTY_LIMIT + restored;
    MP_LOG("[TeamUp] Restored %d/%d non-participating Pokemon (slots %d-%d, prevCount=%d)\n",
                restored, s_savedFullPartyCount - TEAMUP_PARTY_LIMIT,
                TEAMUP_PARTY_LIMIT, s_savedFullPartyCount - 1, prevCount);
}

// Full party restore for abnormal battle endings (comm error, disconnect).
// Restores ALL Pokemon from the pre-battle save — battle data is unreliable.
static void restoreFullParty() {
    if (s_savedFullPartyCount <= 0) return;

    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;

    party->fields.m_memberCount = s_savedFullPartyCount;

    int32_t restored = 0;
    for (int i = 0; i < s_savedFullPartyCount; i++) {
        auto* poke = party->GetMemberPointer(i);
        if (poke != nullptr && poke->fields.m_accessor != nullptr) {
            poke->fields.m_accessor->Deserialize_FullData(
                &s_savedFullPartyBuf[i * POKE_FULL_DATA_SIZE]);
            restored++;
        } else {
            MP_LOG("[TeamUp] WARNING: full restore slot %d poke=%p accessor=%p\n",
                        i, poke, poke ? poke->fields.m_accessor : nullptr);
        }
    }
    MP_LOG("[TeamUp] Full party restore: %d/%d Pokemon rolled back to pre-battle state\n",
                restored, s_savedFullPartyCount);
}

// Save the exp-modified party and write it straight into PlayerWork.
// Called from storeBattleResult AFTER Orig, so ApplyBattlePartyData has already
// deserialized the battle results (exp/level/moves/HP) into the "source party".
//
// The old code read BSP.party[0], which is WRONG: ApplyBattlePartyData writes the
// results to the source party for the local client (MainModule::GetSrcParty), and
// only CopyFrom's them into BSP.party[myClientId] — 2 for Player B, not 0 — and
// even then BSP.party[0] holds the pre-exp copy. So exp never reached PlayerWork
// and evolution (which reads PlayerWork in FinalizeCoroutine) never fired.
//
// GetSrcParty @ 0x20325A0 (mainModule, clientId, flag=1) returns the exp'd party.
// We snapshot it (for the deferred re-apply safety net) AND write it into PlayerWork
// immediately, so the vanilla evolution check later in FinalizeCoroutine sees the
// leveled-up pokemon and plays the evolution demo.
static void saveBattleModifiedParty(void* mainModule, uint8_t myClientId, bool isWin) {
    // NOTE: do NOT zero s_battleModPartyCount here. This runs twice — once at
    // storeBattleResult (guaranteed source data, but the final KO's exp may not
    // have landed yet) and once at Clear (exp complete, but GetSrcParty may be
    // gone). We only commit the buffer once a conversion actually produces
    // members, so a failed second pass can't wipe a good first pass.
    if (mainModule == nullptr) return;

    // Resolve the battle POKECON (holds the real exp/level/moves earned in the fight)
    // + result SendDataContainer, the same way ApplyBattlePartyData does. Comm-mode
    // skips the conversion (gated on IsCompetitorScenarioMode/type==2), so we run it.
    void* inner    = *(void**)((uintptr_t)mainModule + 0x10);
    void* param2   = *(void**)((uintptr_t)mainModule + 0x110);
    void* serverPokecon = (param2 != nullptr) ? *(void**)((uintptr_t)param2 + 0x10) : nullptr;
    // Prefer the pokecon the client-side executor actually applied exp into (the one
    // that leveled up on screen). On the joiner this holds the final KO the server-synced
    // pokecon lags; on the host they're the same object, so this is safe for both.
    void* pokecon  = (s_expApplyPokecon != nullptr) ? s_expApplyPokecon : serverPokecon;
    void* sendData = (inner  != nullptr) ? *(void**)((uintptr_t)inner + 0x118) : nullptr;
    if (pokecon == nullptr) {
        MP_LOG("[TeamUp] saveBattleModifiedParty: pokecon null — cannot apply exp\n");
        return;
    }

    auto* realParty = PlayerWork::get_playerParty();
    if (realParty == nullptr) return;

    // Use myClientId — the game's authoritative designation of THIS client's slot in
    // the comm battle. It reliably indexes the local player's own party in the src
    // array for both initiator and joiner roles. (PID matching was collision-prone:
    // cloned saves and identical species share personality values, so it could match
    // the partner's slot. myClientId can't collide — it IS us.)
    auto* srcParty = (Pml::PokeParty::Object*)_ILExternal::external<void*>(
        0x20325A0, mainModule, (uint32_t)myClientId, 1u);
    if (srcParty == nullptr) {
        // Clear-time: the accessor is torn down, but the object we resolved at
        // storeBattleResult is still alive — reuse it as the conversion dest.
        srcParty = s_captureSrcParty;
    }
    if (srcParty == nullptr) {
        MP_LOG("[TeamUp] saveBattleModifiedParty: GetSrcParty(client=%u) null (no saved party)\n", (unsigned)myClientId);
        return;
    }
    s_captureSrcParty = srcParty;  // remember for the later Clear top-up

    // POKECON::ConvertToPokePartyByStartingOrder(pokecon, destParty, clientId, sendData)
    // Reads the exp'd BTL_POKEPARAMs straight from the POKECON (needs only pokecon
    // + clientId), Clears the dest, and refills it in starting order.
    _ILExternal::external<void>(0x2041840, pokecon, srcParty, (uint32_t)myClientId, sendData);

    // If the conversion produced nothing (e.g. the saved party was torn down after
    // all), bail WITHOUT touching the buffer so we keep the earlier good capture.
    if (srcParty->fields.m_memberCount <= 0) {
        MP_LOG("[TeamUp] saveBattleModifiedParty: conversion yielded 0 members — keeping previous capture\n");
        return;
    }
    s_battleModPartyCount = 0;

    int32_t count = srcParty->fields.m_memberCount;
    if (count > TEAMUP_PARTY_LIMIT) count = TEAMUP_PARTY_LIMIT;

    for (int i = 0; i < count; i++) {
        auto* poke = srcParty->GetMemberPointer(i);
        if (poke != nullptr && poke->fields.m_accessor != nullptr) {
            poke->fields.m_accessor->Serialize_FullData(
                &s_battleModPartyBuf[i * POKE_FULL_DATA_SIZE]);
            s_battleModPartyCount++;

            // Immediate write into the real party so evolution (which reads
            // PlayerWork's party) sees the results before the FinalizeCoroutine check.
            if (realParty != nullptr) {
                auto* dst = realParty->GetMemberPointer(i);
                if (dst != nullptr && dst->fields.m_accessor != nullptr) {
                    dst->fields.m_accessor->Deserialize_FullData(
                        &s_battleModPartyBuf[i * POKE_FULL_DATA_SIZE]);
                }
            }
        }
    }

    // On a WIN, keep the battle HP; but if every mon that ACTUALLY entered the battle
    // fainted, the team won while this player wiped — leave their first BATTLER at 1 HP
    // so they aren't stuck fully fainted after a victory. Use fightPokeIdx (the bool[]
    // the conversion just populated: byte per slot at sendData+0x20+i, length at +0x18)
    // so we only consider mons that were sent out — reviving the "first battle-able" slot
    // would be wrong, since that can be an egg OR a mon that was already KO'd before the
    // battle and never participated. (A LOSS heals the whole party in the deferred
    // restore, so it covers non-participants there.)
    if (isWin && realParty != nullptr && s_battleModPartyCount > 0) {
        int32_t fpiLen = (sendData != nullptr) ? *(int32_t*)((uintptr_t)sendData + 0x18) : 0;
        auto fought = [&](int i) -> bool {
            return i < fpiLen && *(uint8_t*)((uintptr_t)sendData + 0x20 + i) != 0;
        };
        bool anyAlive = false;
        for (int i = 0; i < s_battleModPartyCount; i++) {
            auto* p = realParty->GetMemberPointer(i);
            if (fought(i) && p != nullptr && ((Pml::PokePara::CoreParam*)p)->GetHp() > 0) {
                anyAlive = true; break;
            }
        }
        if (!anyAlive) {
            for (int i = 0; i < s_battleModPartyCount; i++) {
                auto* p = realParty->GetMemberPointer(i);
                if (fought(i) && p != nullptr && p->fields.m_accessor != nullptr) {
                    ((Pml::PokePara::CoreParam*)p)->SetHp(1);
                    p->fields.m_accessor->Serialize_FullData(&s_battleModPartyBuf[i * POKE_FULL_DATA_SIZE]);
                    MP_LOG("[TeamUp] Victory-wipe rescue — revived first battler (slot %d) to 1 HP\n", i);
                    break;
                }
            }
        }
    }
    s_battleWasLoss = !isWin;

    MP_LOG("[TeamUp] Saved+applied %d exp-modified Pokemon (isWin=%d) from GetSrcParty(client=%u)\n",
                s_battleModPartyCount, (int)isWin, (unsigned)myClientId);
}

// Apply deferred party restore to PlayerWork.
// Called from overworldMPHandleTeamUpPostBattle AFTER vanilla finalization,
// and again from the second-pass timer as a safety net.
void applyDeferredPartyRestore() {
    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;

    if (s_battleAbnormal) {
        // Abnormal: restore ALL Pokemon from pre-battle save
        restoreFullParty();
        return;
    }

    // Normal: write battle-modified slots 0-2, then restore saved slots 3-5
    if (s_battleModPartyCount > 0) {
        // Ensure member count covers all slots we'll write
        if (party->fields.m_memberCount < s_savedFullPartyCount) {
            party->fields.m_memberCount = s_savedFullPartyCount;
        }
        int32_t written = 0;
        for (int i = 0; i < s_battleModPartyCount; i++) {
            auto* poke = party->GetMemberPointer(i);
            if (poke != nullptr && poke->fields.m_accessor != nullptr) {
                poke->fields.m_accessor->Deserialize_FullData(
                    &s_battleModPartyBuf[i * POKE_FULL_DATA_SIZE]);
                written++;
            }
        }
        MP_LOG("[TeamUp] Applied %d battle-modified Pokemon to PlayerWork\n", written);
    }
    restoreNonParticipatingPokemon();

    // On a LOSS, the pokémon-center restores the WHOLE party — heal every member to
    // full (HP/PP/status) while keeping the exp/level/move gains applied above. Done
    // here (the last restore step) so nothing re-damages them afterward.
    if (s_battleWasLoss) {
        int32_t total = party->fields.m_memberCount;
        if (total > 6) total = 6;
        for (int i = 0; i < total; i++) {
            auto* poke = party->GetMemberPointer(i);
            if (poke != nullptr) {
                ((Pml::PokePara::CoreParam*)poke)->RecoverAll();
            }
        }
        MP_LOG("[TeamUp] LOSS — full party healed (%d members)\n", total);
    }
}

// ---------------------------------------------------------------------------
// Team-Up handshake
// ---------------------------------------------------------------------------
void overworldMPTeamUp(int32_t partnerStation) {
    s_teamUpState.Clear();
    s_teamUpState.isTeamedUp = true;
    s_teamUpState.partnerStation = partnerStation;
    MP_LOG("[TeamUp] Teamed up with station %d\n", partnerStation);
}

// ---------------------------------------------------------------------------
// Disband
// ---------------------------------------------------------------------------
static void sendTeamUpDisband(int32_t targetStation) {
    if (!isOverworldMPActive()) return;

    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_TEAMUP_DISBAND);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[TeamUp] Sent DISBAND to station %d\n", targetStation);
}

void overworldMPTeamUpDisband() {
    if (!s_teamUpState.isTeamedUp) return;

    int32_t partner = s_teamUpState.partnerStation;
    sendTeamUpDisband(partner);
    s_teamUpState.Clear();
    // Area name window dismissed by releaseDeferredEncount when _updateType returns to 0
    MP_LOG("[TeamUp] Disbanded (was teamed with station %d)\n", partner);
}

void overworldMPOnTeamUpDisbandReceived(int32_t fromStation) {
    if (!s_teamUpState.isTeamedUp) return;
    if (s_teamUpState.partnerStation != fromStation) return;

    s_teamUpState.Clear();
    // Area name window dismissed by releaseDeferredEncount when _updateType returns to 0
    MP_LOG("[TeamUp] Partner station %d disbanded\n", fromStation);
}

// ---------------------------------------------------------------------------
// Sync-wait packet send/receive
// ---------------------------------------------------------------------------

// Send SYNC_WAIT (0xCB) to partner — "I'm waiting at this trainer"
static void sendSyncWait(int32_t targetStation) {
    if (!isOverworldMPActive()) return;

    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    auto& tu = s_teamUpState;

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_TEAMUP_SYNC_WAIT);
    // fromStation is provided by PIA transport header (PR_FROM_STATION) — don't write it
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tu.syncTrainerID); // trainerID
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tu.syncTrainerID2); // trainerID2
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tu.syncArenaID);   // arenaID
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tu.syncWeatherType); // weatherType
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tu.syncEffectID);  // effectID
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tu.syncRandomTeamMode); // randomTeamMode
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tu.syncZoneID);  // zoneID

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[TeamUp] Sent SYNC_WAIT: trainer=%d/%d arena=%d zone=%d randomMode=%d\n",
                tu.syncTrainerID, tu.syncTrainerID2, tu.syncArenaID,
                tu.syncZoneID, tu.syncRandomTeamMode);
}

// Send SYNC_CANCEL (0xCC) to partner — "I gave up / going solo"
static void sendSyncCancel(int32_t targetStation, uint8_t reason) {
    if (!isOverworldMPActive()) return;

    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_TEAMUP_SYNC_CANCEL);
    // fromStation is provided by PIA transport header — don't write it
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, reason);

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[TeamUp] Sent SYNC_CANCEL: reason=%d\n", (int)reason);
}

static void showSyncWaitMessage(); // forward decl (called from tick, defined below DeferEncountStart)
static void hideMsgWindowCustomText(); // forward decl (called from cancel/release)

// Enter sync-wait state: pause event script, send SYNC_WAIT, show waiting message.
void overworldMPEnterSyncWait() {
    auto& tu = s_teamUpState;

    // Capture our randomTeamMode and zone ID at sync entry time
    tu.syncRandomTeamMode = (int32_t)getCustomSaveData()->settings.randomTeamMode;
    tu.syncZoneID = PlayerWork::get_zoneID();

    // Check if partner already sent us a SYNC_WAIT for this trainer
    // (received while we were in SYNC_NONE, stored by onSyncWaitReceived).
    // Only auto-match on exact trainer ID match. Mismatches require manual Y-press.
    bool idsMatch = tu.partnerSyncReceived &&
                    tu.syncTrainerID == tu.battleTrainerID;

    // If partner sent SYNC_WAIT but IDs didn't match,
    // set mismatch flag so tick shows the Y-to-synchronize prompt
    if (tu.partnerSyncReceived && !idsMatch) {
        tu.partnerSyncMismatch = true;
        s_syncMessageShown = false; // force message refresh
    }

    // Copy current battle info into sync fields
    tu.syncTrainerID = tu.battleTrainerID;
    tu.syncTrainerID2 = tu.battleTrainerID2;
    tu.syncArenaID = tu.battleArenaID;
    tu.syncWeatherType = tu.battleWeatherType;
    tu.syncEffectID = tu.battleEffectID;
    tu.syncPhase = SyncPhase::SYNC_WAITING;
    tu.partnerSyncReceived = idsMatch;

    if (idsMatch) {
        MP_LOG("[TeamUp] Partner already waiting — instant match (exact ID)\n");
    }

    // Send SYNC_WAIT to partner
    sendSyncWait(tu.partnerStation);
    s_syncMessageTimer = 0.0f;

    // Don't show UI here — we're inside SetupBattleTrainer (event script context)
    // which crashes FieldCanvas. The tick function will show the message after
    // FieldManager::Update Orig() returns (safe context with FieldCanvas ready).
    if (!tu.partnerSyncMismatch) {
        s_syncMessageShown = false;
    }

    MP_LOG("[TeamUp] Entering SYNC_WAITING: trainer=%d/%d zone=%d randomMode=%d\n",
                tu.syncTrainerID, tu.syncTrainerID2, tu.syncZoneID, tu.syncRandomTeamMode);
}

// True if the local party has at least one battle-eligible (non-egg, HP>0) Pokemon
// within the first TEAMUP_PARTY_LIMIT slots. The comm/PP_AA battle uses those slots
// verbatim, so a player whose first slots are all eggs (e.g. daycare eggs in front)
// or fainted has no usable lead and must not be forced into the link double battle.
// Both players run this check, so a leadless player simply battles solo and never
// pulls a partner in — no separate partner-side check is required.
bool overworldMPLocalHasTeamUpLead() {
    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return false;

    int32_t count = party->fields.m_memberCount;
    if (count > TEAMUP_PARTY_LIMIT) count = TEAMUP_PARTY_LIMIT;

    for (int i = 0; i < count; i++) {
        auto* member = party->GetMemberPointer(i);
        if (member == nullptr) continue;
        auto* core = member->cast<Pml::PokePara::CoreParam>();
        if (core == nullptr) continue;
        if (!core->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG) && !core->IsHpZero()) {
            return true;
        }
    }
    return false;
}

// Cancel sync and proceed with solo battle (original BSP, no PP_AA)
void overworldMPCancelSyncAndGoSolo() {
    auto& tu = s_teamUpState;

    MP_LOG("[TeamUp] Cancelling sync — proceeding solo (was phase %d)\n",
                (int)tu.syncPhase);

    // Notify partner
    if (tu.syncPhase != SyncPhase::SYNC_NONE) {
        sendSyncCancel(tu.partnerStation, 0); // reason=0 manual cancel
    }

    // Clear sync state
    tu.syncPhase = SyncPhase::SYNC_NONE;
    tu.partnerSyncReceived = false;
    tu.partnerSyncMismatch = false;
    tu.partnerRandomTeamMode = -1;
    tu.partnerZoneID = -1;
    tu.bypassTrainerFlag = false;
    tu.battlePending = false;
    // A cancelled sync is never a team-up battle — make sure a stale "partner party
    // received" flag from a previous battle doesn't promote this solo release.
    tu.partnerPartyValid = false;

    // Hide MsgWindow — releaseDeferredEncount will also try, but safe to double-call
    hideMsgWindowCustomText();
}

// Called from packet handler when TEAMUP_SYNC_WAIT (0xCB) arrives
void overworldMPOnSyncWaitReceived(int32_t fromStation, int32_t trainerID,
                                    int32_t trainerID2, int32_t arenaID,
                                    int32_t weatherType, int32_t effectID,
                                    int32_t randomTeamMode, int32_t zoneID) {
    auto& tu = s_teamUpState;
    if (!tu.isTeamedUp || tu.partnerStation != fromStation) {
        MP_LOG("[TeamUp] Ignoring SYNC_WAIT from non-partner station %d\n", fromStation);
        return;
    }

    // If we're already in battle, ignore
    if (overworldMPIsInBattleScene()) {
        MP_LOG("[TeamUp] Ignoring SYNC_WAIT: already in battle\n");
        return;
    }

    MP_LOG("[TeamUp] Received SYNC_WAIT: trainer=%d/%d zone=%d randomMode=%d from station %d (our phase=%d)\n",
                trainerID, trainerID2, zoneID, randomTeamMode, fromStation, (int)tu.syncPhase);

    if (tu.syncPhase == SyncPhase::SYNC_WAITING) {
        // We're also waiting — only auto-match on exact trainer ID match.
        // Mismatches require manual Y-press to synchronize.
        bool idsMatch = (tu.syncTrainerID == trainerID);

        if (idsMatch) {
            MP_LOG("[TeamUp] SYNC MATCH! (exact trainer ID %d)\n", trainerID);
            tu.partnerSyncReceived = true;
            // The tick function will detect this and transition to SYNC_MATCHED
        } else {
            MP_LOG("[TeamUp] Mismatch: us=%d partner=%d — awaiting Y-to-sync\n",
                        tu.syncTrainerID, trainerID);
            tu.partnerSyncMismatch = true;
            tu.partnerRandomTeamMode = randomTeamMode;
            tu.partnerZoneID = zoneID;
            s_syncMessageShown = false; // force message refresh
        }
    } else if (tu.syncPhase == SyncPhase::SYNC_NONE) {
        // We're not in sync yet — partner is waiting for us at a trainer.
        // Store the info so when we trigger the same trainer, we can detect the match.
        // Also enable trainer flag bypass so Player B can re-fight defeated trainers.
        tu.partnerSyncReceived = true;
        tu.syncTrainerID = trainerID;
        tu.syncTrainerID2 = trainerID2;
        tu.syncArenaID = arenaID;
        tu.syncWeatherType = weatherType;
        tu.syncEffectID = effectID;
        tu.partnerRandomTeamMode = randomTeamMode;
        tu.partnerZoneID = zoneID;
        tu.bypassTrainerFlag = true;

        MP_LOG("[TeamUp] Partner waiting at trainer %d zone=%d — bypass enabled\n",
                    trainerID, zoneID);
    }
    // If SYNC_MATCHED, ignore (already matched or in exchange)
}

// Called from packet handler when TEAMUP_SYNC_CANCEL (0xCC) arrives
void overworldMPOnSyncCancelReceived(int32_t fromStation, uint8_t reason) {
    auto& tu = s_teamUpState;
    if (!tu.isTeamedUp || tu.partnerStation != fromStation) return;

    MP_LOG("[TeamUp] Received SYNC_CANCEL: reason=%d from station %d (our phase=%d)\n",
                (int)reason, fromStation, (int)tu.syncPhase);

    if (tu.syncPhase == SyncPhase::SYNC_WAITING) {
        // Partner cancelled — we go solo too
        // The tick function will detect syncPhase==SYNC_NONE and release DeferEncountStart
        tu.syncPhase = SyncPhase::SYNC_NONE;
        tu.partnerSyncReceived = false;
        tu.bypassTrainerFlag = false;
        // Area name window dismissed by releaseDeferredEncount when _updateType returns to 0
        MP_LOG("[TeamUp] Partner cancelled sync — going solo\n");
    } else if (tu.syncPhase == SyncPhase::SYNC_NONE) {
        // We weren't syncing — clear any stored partner sync info
        tu.partnerSyncReceived = false;
        tu.bypassTrainerFlag = false;
    }
    // If SYNC_MATCHED, too late to cancel (exchange in progress) — ignore
}

// ---------------------------------------------------------------------------
// Auto-disband check (called each tick)
// ---------------------------------------------------------------------------
void overworldMPTeamUpAutoDisband() {
    if (!s_teamUpState.isTeamedUp) return;

    // Race condition #5: don't auto-disband while in an active battle —
    // clearing TeamUpState mid-battle would lose battleType/isInitiator,
    // causing TeamUpStoreBattleResult to skip party copy-back.
    if (overworldMPIsInBattleScene()) return;

    // NPC companion active (Riley, Cheryl, etc.) → disband
    if (PlayerWork::GetSystemFlag((int32_t)FlagWork_SysFlag::SYS_FLAG_PAIR)) {
        MP_LOG("[TeamUp] Auto-disband: SYS_FLAG_PAIR is set\n");
        overworldMPTeamUpDisband();
        return;
    }

    // Partner disconnected → disband
    auto& ctx = getOverworldMPContext();
    int32_t ps = s_teamUpState.partnerStation;
    if (ps < 0 || ps >= OW_MP_MAX_PLAYERS || !ctx.remotePlayers[ps].isActive) {
        MP_LOG("[TeamUp] Auto-disband: partner station %d disconnected\n", ps);

        // Clean up sync/battle state on disconnect
        if (s_teamUpState.syncPhase != SyncPhase::SYNC_NONE || s_teamUpState.battlePending) {
            s_teamUpBSP = nullptr;
            s_teamUpEncountPending = false;
            s_teamUpState.battlePending = false;
            s_teamUpState.syncPhase = SyncPhase::SYNC_NONE;
            s_teamUpState.bypassTrainerFlag = false;
            // Display message will be closed by releaseDeferredEncount
            if (s_encountDeferred) {
                // Cancel deferral — tick will detect syncPhase=SYNC_NONE and release
                // EncountStart with original trainer BSP as fallback.
                MP_LOG("[TeamUp] Partner disconnected during sync/deferred EncountStart — "
                            "tick will release\n");
            }
        }

        s_teamUpState.Clear();
        return;
    }

    // Race condition #1: battlePending timeout for the non-deferred case.
    // If battlePending is true but the deferred EncountStart didn't fire
    // (e.g., battle already started before ACK arrived in low-latency case),
    // time it out after DEFERRED_ENCOUNT_TIMEOUT seconds.
    static float s_battlePendingTimer = -1.0f;
    if (s_teamUpState.battlePending && !s_encountDeferred) {
        if (s_battlePendingTimer < 0.0f) {
            s_battlePendingTimer = DEFERRED_ENCOUNT_TIMEOUT;
        }
        s_battlePendingTimer -= UnityEngine::Time::get_deltaTime();
        if (s_battlePendingTimer <= 0.0f) {
            MP_LOG("[TeamUp] battlePending timeout (non-deferred) — clearing\n");
            s_teamUpState.battlePending = false;
            s_teamUpBSP = nullptr;
            s_battlePendingTimer = -1.0f;
        }
    } else {
        s_battlePendingTimer = -1.0f;
    }
}

// ---------------------------------------------------------------------------
// Send party data using chunked protocol (reuses 0xC6 pattern)
// dataId = OWMP_DATA_ID_TEAMUP_BATTLE or OWMP_DATA_ID_TEAMUP_BATTLE_ACK
// ---------------------------------------------------------------------------
static void sendTeamUpPartyChunked(int32_t targetStation, uint8_t dataId,
                                    uint8_t battleType, int32_t arenaID,
                                    int32_t weatherType, int32_t trainerID) {
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;

    int32_t memberCount = party->fields.m_memberCount;
    if (memberCount > TEAMUP_PARTY_LIMIT) memberCount = TEAMUP_PARTY_LIMIT;

    // --- HEADER sub-packet ---
    // Format: [dataId][target:4][sub:HEADER][battleType:1][memberCount:1][trainerMemberCount:1]
    //         [arenaID:4][weatherType:4][trainerID:4][trainerID2:4][effectID:4][MYSTATUS fields]
    // trainerMemberCount: how many TEAMUP_SUB_TRAINER_POKE packets follow (0 for ACK)

    // Count trainer party members from BSP (BATTLE sends initiator's trainer,
    // ACK sends Player B's trainer for dual-trainer mode)
    extern Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* s_teamUpBSP;
    uint8_t trainerMemberCount = 0;
    if (s_teamUpBSP != nullptr) {
        auto* bspFields = &s_teamUpBSP->instance()->fields;
        if (bspFields->party != nullptr && bspFields->party->max_length > 1) {
            auto* trainerParty = bspFields->party->m_Items[1];
            if (trainerParty != nullptr) {
                trainerMemberCount = (uint8_t)trainerParty->fields.m_memberCount;
                if (trainerMemberCount > 6) trainerMemberCount = 6;
            }
        }
    }

    // Second real trainer's party (BSP slot 3) — only for a genuine two-trainer double
    // (battleTrainerID2 != 0), and only in the BATTLE packet (the joiner never sends it).
    uint8_t trainerMemberCount2 = 0;
    if (dataId == OWMP_DATA_ID_TEAMUP_BATTLE &&
        overworldMPGetTeamUpState().battleTrainerID2 != 0 && s_teamUpBSP != nullptr) {
        auto* bspFields = &s_teamUpBSP->instance()->fields;
        if (bspFields->party != nullptr && bspFields->party->max_length > 3) {
            auto* trainerParty2 = bspFields->party->m_Items[3];
            if (trainerParty2 != nullptr) {
                trainerMemberCount2 = (uint8_t)trainerParty2->fields.m_memberCount;
                if (trainerMemberCount2 > 6) trainerMemberCount2 = 6;
            }
        }
    }

    // Decide the battle's multiMode (initiator is authoritative; the BATTLE packet
    // carries it and the joiner obeys — this keeps both consoles' multiMode identical,
    // which a divergent per-side decision would not). PA_A2 (single enemy trainer
    // covering both positions) only for a genuine, matched single-trainer encounter:
    // no pre-existing second trainer (battleTrainerID2==0) and the sync matched
    // (not a dual-trainer merge of two different encounters).
    auto& tuMode = overworldMPGetTeamUpState();
    uint8_t battleMultiMode = TEAMUP_MULTIMODE_PP_AA;
    if (dataId == OWMP_DATA_ID_TEAMUP_BATTLE &&
        tuMode.battleTrainerID2 == 0 && !tuMode.partnerSyncMismatch) {
        battleMultiMode = TEAMUP_MULTIMODE_SINGLE;
    } else if (dataId == OWMP_DATA_ID_TEAMUP_BATTLE_ACK) {
        // Joiner echoes the mode it was told, so the BATTLE sender can confirm it.
        battleMultiMode = tuMode.battleMultiMode ? tuMode.battleMultiMode
                                                 : TEAMUP_MULTIMODE_PP_AA;
    }
    tuMode.battleMultiMode = battleMultiMode;

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, dataId);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, TEAMUP_SUB_HEADER);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, battleType);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)memberCount);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, trainerMemberCount);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, trainerMemberCount2);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, battleMultiMode);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, arenaID);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, weatherType);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, trainerID);

    // Write trainerID2 and battleEffectID (from TeamUpState)
    auto& tuSend = overworldMPGetTeamUpState();
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tuSend.battleTrainerID2);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tuSend.battleEffectID);

    // Write MYSTATUS_COMM
    auto* playerStatus = PlayerWork::get_playerStatus();
    if (playerStatus != nullptr) {
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, (int32_t)playerStatus->fields.id);
        auto* nameStr = playerStatus->fields.name;
        if (nameStr != nullptr) {
            int nameLen = nameStr->fields.m_stringLength;
            if (nameLen > 12) nameLen = 12;
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)nameLen);
            for (int i = 0; i < nameLen * 2; i++) {
                uint8_t ch = ((uint8_t*)&nameStr->fields.m_firstChar)[i];
                il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, ch);
            }
        } else {
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0);
        }
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.sex ? 1 : 0);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.region_code);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.fashion);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.body_type);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0); // hat
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0); // shoes
    } else {
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, 0);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0);
        for (int i = 0; i < 6; i++)
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0);
    }

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[TeamUp] Sent %s HEADER: target=%d members=%d trainerMembers=%d battle=%d\n",
                dataId == OWMP_DATA_ID_TEAMUP_BATTLE ? "BATTLE" : "ACK",
                targetStation, memberCount, (int)trainerMemberCount, (int)battleType);

    // --- Per-Pokemon sub-packets ---
    for (int32_t i = 0; i < memberCount; i++) {
        auto* poke = party->GetMemberPointer(i);
        uint8_t pokeBuf[POKE_FULL_DATA_SIZE];
        memset(pokeBuf, 0, sizeof(pokeBuf));

        if (poke != nullptr && poke->fields.m_accessor != nullptr) {
            poke->fields.m_accessor->Serialize_FullData( pokeBuf);
        }

        il2cpp_vcall_void(pw, PW_RESET);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, dataId);
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, TEAMUP_SUB_POKE);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)i);

        for (int j = 0; j < POKE_FULL_DATA_INTS; j++) {
            int32_t val = 0;
            memcpy(&val, &pokeBuf[j * 4], 4);
            il2cpp_vcall_write_s32(pw, PW_WRITE_S32, val);
        }

        Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
        MP_LOG("[TeamUp] Sent %s POKE[%d]\n",
                    dataId == OWMP_DATA_ID_TEAMUP_BATTLE ? "BATTLE" : "ACK", i);
    }

    // --- Trainer party sub-packets (BATTLE only, not ACK) ---
    // Send the initiator's trainer party so the non-initiator uses identical enemy Pokemon
    // regardless of per-save randomization differences.
    if (trainerMemberCount > 0 && s_teamUpBSP != nullptr) {
        auto* bspFields = &s_teamUpBSP->instance()->fields;
        auto* trainerParty = bspFields->party->m_Items[1]; // slot 1 = trainer's full party from Orig

        for (int32_t i = 0; i < trainerMemberCount; i++) {
            auto* poke = trainerParty->GetMemberPointer(i);
            uint8_t pokeBuf[POKE_FULL_DATA_SIZE];
            memset(pokeBuf, 0, sizeof(pokeBuf));

            if (poke != nullptr && poke->fields.m_accessor != nullptr) {
                poke->fields.m_accessor->Serialize_FullData( pokeBuf);
            }

            il2cpp_vcall_void(pw, PW_RESET);
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, dataId);
            il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, TEAMUP_SUB_TRAINER_POKE);
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)i);

            for (int j = 0; j < POKE_FULL_DATA_INTS; j++) {
                int32_t val = 0;
                memcpy(&val, &pokeBuf[j * 4], 4);
                il2cpp_vcall_write_s32(pw, PW_WRITE_S32, val);
            }

            Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
            MP_LOG("[TeamUp] Sent BATTLE TRAINER_POKE[%d]\n", i);
        }
    }

    // --- Second real trainer's party (slot 3) — genuine two-trainer double only ---
    // Same per-poke chunking as slot 1, tagged TEAMUP_SUB_TRAINER_POKE2.
    if (trainerMemberCount2 > 0 && s_teamUpBSP != nullptr) {
        auto* bspFields = &s_teamUpBSP->instance()->fields;
        auto* trainerParty2 = bspFields->party->m_Items[3];

        for (int32_t i = 0; i < trainerMemberCount2; i++) {
            auto* poke = trainerParty2->GetMemberPointer(i);
            uint8_t pokeBuf[POKE_FULL_DATA_SIZE];
            memset(pokeBuf, 0, sizeof(pokeBuf));

            if (poke != nullptr && poke->fields.m_accessor != nullptr) {
                poke->fields.m_accessor->Serialize_FullData(pokeBuf);
            }

            il2cpp_vcall_void(pw, PW_RESET);
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, dataId);
            il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, TEAMUP_SUB_TRAINER_POKE2);
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)i);

            for (int j = 0; j < POKE_FULL_DATA_INTS; j++) {
                int32_t val = 0;
                memcpy(&val, &pokeBuf[j * 4], 4);
                il2cpp_vcall_write_s32(pw, PW_WRITE_S32, val);
            }

            Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
            MP_LOG("[TeamUp] Sent BATTLE TRAINER_POKE2[%d]\n", i);
        }
    }
}

// ---------------------------------------------------------------------------
// Internal send functions
// ---------------------------------------------------------------------------
static void overworldMPSendTeamUpBattleTrainer(int32_t targetStation,
                                                int32_t enemyID0, int32_t arenaID,
                                                int32_t weatherType) {
    sendTeamUpPartyChunked(targetStation, OWMP_DATA_ID_TEAMUP_BATTLE,
                           1, arenaID, weatherType, enemyID0);
}

void overworldMPSendTeamUpBattleAck(int32_t targetStation) {
    // ACK sends our party + our trainer ID for dual-trainer mode
    auto& tu = overworldMPGetTeamUpState();
    sendTeamUpPartyChunked(targetStation, OWMP_DATA_ID_TEAMUP_BATTLE_ACK,
                           0, 0, 0, tu.syncTrainerID);
}

// ---------------------------------------------------------------------------
// Deserialize partner party from accumulated buffer
// (Same logic as overworld_mp_interact.cpp::deserializeBattleParty)
// ---------------------------------------------------------------------------
static Pml::PokeParty::Object* deserializeTeamUpParty(uint8_t* buf, int32_t size, int32_t count) {
    if (count <= 0 || count > 6) return nullptr;

    auto* localParty = PlayerWork::get_playerParty();
    if (localParty == nullptr) return nullptr;

    // Reuse the existing party's class + raw ctor instead of newInstance():
    // newInstance's generic-TypeInfo lazy class-init null-derefs in the battle
    // setup context. Load-bearing pattern — do not "clean up" to newInstance().
    auto* party = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)localParty->klass);
    _ILExternal::external<void>(0x2055D10, party); // PokeParty::ctor()

    int32_t validCount = 0;
    for (int i = 0; i < count; i++) {
        int32_t offset = i * POKE_FULL_DATA_SIZE;
        if (offset + POKE_FULL_DATA_SIZE > size) break;

        auto* slotPoke = party->GetMemberPointer(i);
        if (slotPoke == nullptr) continue;

        auto* accessor = slotPoke->fields.m_accessor;
        if (accessor != nullptr) {
            accessor->Deserialize_FullData( &buf[offset]);
        }

        // Validate (shared range check — see mp_poke_validate.h). Stop at the first
        // invalid member so the party stays a contiguous run of valid Pokemon.
        if (!mpValidatePokemonParam((Pml::PokePara::CoreParam*)slotPoke)) {
            MP_LOG("[TeamUp] REJECT partner poke[%d] — failed validation\n", i);
            break;
        }
        validCount++;
    }

    if (validCount == 0) return nullptr;
    party->fields.m_memberCount = validCount;
    MP_LOG("[TeamUp] Deserialized partner party: %d members\n", validCount);
    return party;
}

// ---------------------------------------------------------------------------
// Overwrite a BSP party slot's Pokemon with trainer data received from initiator.
// normalTrainer must have been called first (so party[slot] and tr_data[slot] exist).
// This replaces the Pokemon data while keeping BSP_TRAINER_DATA (AI, class, etc.) intact.
// ---------------------------------------------------------------------------
static void overwriteTrainerPartyFromBufferEx(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp,
                                              int slot, const uint8_t* buf, int32_t bufSize, int32_t count) {
    auto* fields = &bsp->instance()->fields;
    if (fields->party == nullptr || (uint32_t)slot >= fields->party->max_length) return;

    auto* party = fields->party->m_Items[slot];
    if (party == nullptr) return;

    if (count <= 0 || count > 6 || buf == nullptr) return;

    int32_t validCount = 0;
    for (int i = 0; i < count; i++) {
        int32_t bufOffset = i * POKE_FULL_DATA_SIZE;
        if (bufOffset + POKE_FULL_DATA_SIZE > bufSize) break;

        auto* poke = party->GetMemberPointer(i);
        if (poke == nullptr || poke->fields.m_accessor == nullptr) break;

        poke->fields.m_accessor->Deserialize_FullData(&buf[bufOffset]);

        // Validate the injected (peer-supplied) trainer Pokemon, just like the human
        // party path. Without this, a malicious peer could write arbitrary serialized
        // PokemonParam bytes into the enemy AI slots. Truncate at the first invalid
        // member so the AI party stays a contiguous run of well-formed Pokemon.
        if (!mpValidatePokemonParam((Pml::PokePara::CoreParam*)poke)) {
            MP_LOG("[TeamUp] REJECT trainer poke[%d] — truncating trainer party\n", i);
            break;
        }
        validCount++;
    }
    party->fields.m_memberCount = validCount;

    MP_LOG("[TeamUp] Overwrote party[%d] with %d/%d valid trainer Pokemon\n", slot, validCount, count);
}

// Slot-1 convenience wrapper (initiator's first trainer party).
static void overwriteTrainerPartyFromBuffer(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp,
                                             int slot, TeamUpState& tu) {
    overwriteTrainerPartyFromBufferEx(bsp, slot, tu.trainerPartyBuf, tu.trainerPartyBufSize, tu.trainerPartyCount);
}

// Deserialize MYSTATUS_COMM from buffer at given offset
static MYSTATUS_COMM::Object deserializeTeamUpStatus(uint8_t* buf, int32_t size, int32_t& offset) {
    MYSTATUS_COMM::Object status = {};

    if (offset + 4 > size) return status;
    memcpy(&status.fields.id, &buf[offset], 4);
    offset += 4;

    if (offset + 1 > size) return status;
    uint8_t nameLen = buf[offset++];
    if (nameLen > 0 && nameLen <= 12 && offset + nameLen * 2 <= size) {
        status.fields.name = System::String::fromUnicodeBytes(&buf[offset], nameLen * 2);
        offset += nameLen * 2;
    } else {
        status.fields.name = System::String::Create("Trainer");
    }

    if (offset + 6 > size) return status;
    status.fields.sex = buf[offset++] != 0;
    status.fields.lang = buf[offset++];
    status.fields.fashion = buf[offset++];
    status.fields.body_type = buf[offset++];
    status.fields.hat = buf[offset++];
    status.fields.shoes = buf[offset++];

    return status;
}

// ---------------------------------------------------------------------------
// Apply the human-trainer color overrides for a PP_AA team-up battle. Shared by
// the Player A and Player B BSP setup paths, which are identical apart from the
// slot layout (localSlot/partnerSlot) and the source of the trainer-data /
// player-status pointers. The two AI slots (1,3) keep colorID 0.
//   trData     — bsp fields' tr_data array (caller supplies its own fields->tr_data)
//   statusArr  — the playerStatus array (Player B re-fetches via PlayerWork)
//   remote     — partner's net data (custom colors / colorId)
//   localColor — our save-data colorID (-1 = custom)
//   localSlot / partnerSlot — PP_AA slots (Player A: 0/2, Player B: 2/0)
// ---------------------------------------------------------------------------
static void applyTeamUpBattleColors(void* trData,
                                    Dpr::Battle::Logic::MyStatus::Array* statusArr,
                                    FieldPlayerNetData& remote, int32_t localColor,
                                    int32_t localSlot, int32_t partnerSlot) {
    int32_t partnerColor = remote.colorId; // -1 passes through for custom colors

    // (1) TRAINER_DATA.colorID — human slots take our/partner color, AI slots unchanged.
    if (trData != nullptr) {
        uint32_t arrLen = *(uint32_t*)((uintptr_t)trData + 0x18);
        auto setTd = [&](int32_t slot, int32_t color) {
            if (arrLen > (uint32_t)slot) {
                auto* td = *(Dpr::Battle::Logic::TRAINER_DATA::Object**)
                    ((uintptr_t)trData + 0x20 + slot * 8);
                if (td != nullptr) td->fields.colorID = color;
            }
        };
        setTd(localSlot, localColor);
        setTd(partnerSlot, partnerColor);
    }

    // (2) MyStatus.colorID (byte at object offset 0x25) — write it directly per
    // slot AND register the pointers for the hook. MyStatusGetColorID reads this
    // field; without the byte write the partner's hair/eye color resolves to
    // local/default (b8678d4 regressed this to hook-only registration). Keep both.
    extern void owmpSetBattleMyStatus(int32_t slot, void* myStatus);
    extern void owmpClearBattleMyStatus();
    owmpClearBattleMyStatus();
    if (statusArr != nullptr) {
        if (statusArr->max_length > (uint32_t)localSlot && statusArr->m_Items[localSlot] != nullptr) {
            *(uint8_t*)((uintptr_t)statusArr->m_Items[localSlot] + MYSTATUS_COLORID_OFFSET) = (uint8_t)localColor;
            owmpSetBattleMyStatus(localSlot, statusArr->m_Items[localSlot]);
        }
        if (statusArr->max_length > (uint32_t)partnerSlot && statusArr->m_Items[partnerSlot] != nullptr) {
            *(uint8_t*)((uintptr_t)statusArr->m_Items[partnerSlot] + MYSTATUS_COLORID_OFFSET) = (uint8_t)partnerColor;
            owmpSetBattleMyStatus(partnerSlot, statusArr->m_Items[partnerSlot]);
        }
    }

    // (3) Slot color array + cursor for CardModelViewController
    extern int32_t g_owmpBattleSlotColors[];
    extern int32_t g_owmpBattleSlotCursor;
    g_owmpBattleSlotColors[localSlot]   = localColor;
    g_owmpBattleSlotColors[partnerSlot] = partnerColor;
    g_owmpBattleSlotColors[1] = 0;  // enemy NPC
    g_owmpBattleSlotColors[3] = 0;  // enemy NPC
    g_owmpBattleSlotCursor = 0;
    extern int32_t g_owmpSetSkinColorCursor;
    g_owmpSetSkinColorCursor = 0;
    // Human slots in ascending (render) order so the per-model color cursor maps to
    // the right slot — team-up humans are at localSlot/partnerSlot (0 and 2), not 0/1.
    extern int32_t g_owmpBattleHumanSlots[2];
    g_owmpBattleHumanSlots[0] = (localSlot < partnerSlot) ? localSlot : partnerSlot;
    g_owmpBattleHumanSlots[1] = (localSlot < partnerSlot) ? partnerSlot : localSlot;
    extern bool g_owmpBattleIsTeamUp;
    g_owmpBattleIsTeamUp = true;

    // (4) Custom battle colors for slots with colorId == -1
    extern bool g_owmpBattleSlotHasCustomColors[];
    extern RomData::ColorSet g_owmpBattleSlotCustomColorSets[];
    memset(g_owmpBattleSlotHasCustomColors, 0, sizeof(bool) * 4);

    // Local player — build from save data directly (don't call GetCustomColorSet
    // here, it can crash during BSP setup).
    if (localColor == -1) {
        g_owmpBattleSlotHasCustomColors[localSlot] = true;
        auto& lcs = g_owmpBattleSlotCustomColorSets[localSlot];
        auto& cv = getCustomSaveData()->playerColorVariation;
        lcs.fieldSkinFace  = { cv.fSkinFace.fields.r,  cv.fSkinFace.fields.g,  cv.fSkinFace.fields.b,  cv.fSkinFace.fields.a };
        lcs.fieldSkinMouth = { cv.fSkinMouth.fields.r, cv.fSkinMouth.fields.g, cv.fSkinMouth.fields.b, cv.fSkinMouth.fields.a };
        lcs.fieldEyes      = { cv.fEyes.fields.r,      cv.fEyes.fields.g,      cv.fEyes.fields.b,      cv.fEyes.fields.a };
        lcs.fieldEyebrows  = { cv.fEyebrows.fields.r,  cv.fEyebrows.fields.g,  cv.fEyebrows.fields.b,  cv.fEyebrows.fields.a };
        lcs.fieldSkinBody  = { cv.fSkinBody.fields.r,  cv.fSkinBody.fields.g,  cv.fSkinBody.fields.b,  cv.fSkinBody.fields.a };
        lcs.fieldHair      = { cv.fHair.fields.r,      cv.fHair.fields.g,      cv.fHair.fields.b,      cv.fHair.fields.a };
        lcs.battleSkinFace = { cv.bSkinFace.fields.r,  cv.bSkinFace.fields.g,  cv.bSkinFace.fields.b,  cv.bSkinFace.fields.a };
        lcs.battleHairExtra= { cv.bHairExtra.fields.r, cv.bHairExtra.fields.g, cv.bHairExtra.fields.b, cv.bHairExtra.fields.a };
        lcs.battleEyeLeft  = { cv.bEyeLeft.fields.r,   cv.bEyeLeft.fields.g,   cv.bEyeLeft.fields.b,   cv.bEyeLeft.fields.a };
        lcs.battleEyeRight = { cv.bEyeRight.fields.r,  cv.bEyeRight.fields.g,  cv.bEyeRight.fields.b,  cv.bEyeRight.fields.a };
        lcs.battleSkinBody = { cv.bSkinBody.fields.r,  cv.bSkinBody.fields.g,  cv.bSkinBody.fields.b,  cv.bSkinBody.fields.a };
        lcs.battleHair     = { cv.bHair.fields.r,      cv.bHair.fields.g,      cv.bHair.fields.b,      cv.bHair.fields.a };
    }

    // Partner — from received 0xCD packet data
    if (partnerColor == -1 && remote.hasCustomColors) {
        g_owmpBattleSlotHasCustomColors[partnerSlot] = true;
        auto& cs = g_owmpBattleSlotCustomColorSets[partnerSlot];
        for (int c = 0; c < 6; c++) {
            float* dst = (c == 0) ? &cs.fieldSkinFace.r :
                         (c == 1) ? &cs.fieldSkinMouth.r :
                         (c == 2) ? &cs.fieldEyes.r :
                         (c == 3) ? &cs.fieldEyebrows.r :
                         (c == 4) ? &cs.fieldSkinBody.r : &cs.fieldHair.r;
            dst[0] = remote.customFieldColors[c * 3];
            dst[1] = remote.customFieldColors[c * 3 + 1];
            dst[2] = remote.customFieldColors[c * 3 + 2];
            dst[3] = 1.0f;
        }
        float* battleBase = &cs.battleSkinFace.r;
        for (int c = 0; c < 6; c++) {
            battleBase[c * 4 + 0] = remote.customBattleColors[c * 3];
            battleBase[c * 4 + 1] = remote.customBattleColors[c * 3 + 1];
            battleBase[c * 4 + 2] = remote.customBattleColors[c * 3 + 2];
            battleBase[c * 4 + 3] = 1.0f;
        }
    }
}

// Diagnostic: log the PA_A2 (mode 6) enemy topology so we can verify at runtime that
// client 1 covers BOTH enemy positions and client 2 (the comm ally) is on the player
// side. GetPosCoverClientId_Double @0x20CEF00 (multiMode, pos) -> covering client id.
// Expected for PA_A2: pos0->0, pos1->1, pos2->2, pos3->1.
static void owmpTeamUpLogMode6Topology(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp) {
    (void)bsp;
    uint8_t c[4];
    for (uint8_t pos = 0; pos < 4; pos++) {
        c[pos] = _ILExternal::external<uint8_t>(0x20CEF00,
            (uint8_t)TEAMUP_MULTIMODE_SINGLE, pos, (void*)nullptr);
    }
    MP_LOG("[TeamUp] PA_A2 cover: pos0->c%d pos1->c%d pos2->c%d pos3->c%d (want 0,1,2,1)\n",
                (int)c[0], (int)c[1], (int)c[2], (int)c[3]);
}

// ---------------------------------------------------------------------------
// Modify BSP for PP_AA team-up battle
// ---------------------------------------------------------------------------
void overworldMPModifyBSPForTeamUp(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp,
                                    TeamUpState& tu, bool isInitiator) {
    auto* fields = &bsp->instance()->fields;

    int32_t myStation = mpThisStationIndex(); // ThisStationIndex

    // 1. Set comm battle mode
    fields->commMode = 1;    // BTL_COMM_WIRELESS
    fields->multiMode = 2;   // BTL_MULTIMODE_PP_AA (slots 0,2 = human, 1,3 = AI)
    fields->competitor = (Dpr::Battle::Logic::BtlCompetitor)3; // BTL_COMPETITOR_COMM

    if (isInitiator) {
        fields->commPos = 0;  // We are slot 0 (PLAYER)
    } else {
        fields->commPos = 2;  // We are slot 2 (PARTNER)
    }

    // 2. Set stations array
    if (fields->stations != nullptr && fields->stations->max_length >= 4) {
        if (isInitiator) {
            fields->stations->m_Items[0] = myStation;
            fields->stations->m_Items[1] = -1;  // AI
            fields->stations->m_Items[2] = tu.partnerStation;
            fields->stations->m_Items[3] = -1;  // AI
        } else {
            fields->stations->m_Items[0] = tu.partnerStation;
            fields->stations->m_Items[1] = -1;  // AI
            fields->stations->m_Items[2] = myStation;
            fields->stations->m_Items[3] = -1;  // AI
        }
    } else {
        MP_LOG("[TeamUp] WARNING: stations array too small or null (max_length=%d)\n",
                    fields->stations ? (int)fields->stations->max_length : -1);
    }

    // 3. Inject partner party into slot 2 (or slot 0 for non-initiator)
    int32_t partnerSlot = isInitiator ? 2 : 0;
    if (fields->party != nullptr && fields->party->max_length > (uint64_t)partnerSlot) {
        auto* partnerParty = deserializeTeamUpParty(
            tu.partnerPartyBuf, tu.partnerPartyBufSize, tu.partnerPartyCount);
        if (partnerParty != nullptr && fields->party->m_Items[partnerSlot] != nullptr) {
            // Copy partner's Pokemon into the BSP party slot
            auto* destParty = fields->party->m_Items[partnerSlot];
            int32_t count = partnerParty->fields.m_memberCount;
            if (count > TEAMUP_PARTY_LIMIT) count = TEAMUP_PARTY_LIMIT;

            for (int i = 0; i < count; i++) {
                auto* srcPoke = partnerParty->GetMemberPointer(i);
                auto* dstPoke = destParty->GetMemberPointer(i);
                if (srcPoke != nullptr && dstPoke != nullptr &&
                    srcPoke->fields.m_accessor != nullptr && dstPoke->fields.m_accessor != nullptr) {
                    uint8_t tmpBuf[POKE_FULL_DATA_SIZE];
                    srcPoke->fields.m_accessor->Serialize_FullData( tmpBuf);
                    dstPoke->fields.m_accessor->Deserialize_FullData( tmpBuf);
                }
            }
            destParty->fields.m_memberCount = count;
            MP_LOG("[TeamUp] Injected partner party into slot %d (%d members)\n",
                        partnerSlot, count);
        }
    }

    // 4. Inject partner MYSTATUS into the correct slot
    if (fields->playerStatus != nullptr && fields->playerStatus->max_length > (uint64_t)partnerSlot) {
        int32_t mystOff = 0;
        MYSTATUS_COMM::Object partnerCommStatus = deserializeTeamUpStatus(
            tu.partnerMystatusBuf, tu.partnerMystatusLen, mystOff);

        auto* statusSlot = fields->playerStatus->m_Items[partnerSlot];
        if (statusSlot != nullptr) {
            // Copy fields from MYSTATUS_COMM into MyStatus (different layouts)
            statusSlot->fields.name = partnerCommStatus.fields.name;
            statusSlot->fields.sex = partnerCommStatus.fields.sex;
            statusSlot->fields.lang = partnerCommStatus.fields.lang;
            statusSlot->fields.id = partnerCommStatus.fields.id;
            statusSlot->fields.fashion = partnerCommStatus.fields.fashion;
            statusSlot->fields.body_type = partnerCommStatus.fields.body_type;
            statusSlot->fields.hat = partnerCommStatus.fields.hat;
            statusSlot->fields.shoes = partnerCommStatus.fields.shoes;
            MP_LOG("[TeamUp] Injected partner MYSTATUS into slot %d\n", partnerSlot);
        }
    }

    MP_LOG("[TeamUp] BSP modified: commMode=%d multiMode=%d commPos=%d stations=[%d,%d,%d,%d]\n",
                (int)fields->commMode, (int)fields->multiMode, (int)fields->commPos,
                fields->stations ? fields->stations->m_Items[0] : -99,
                fields->stations ? fields->stations->m_Items[1] : -99,
                fields->stations ? fields->stations->m_Items[2] : -99,
                fields->stations ? fields->stations->m_Items[3] : -99);
}

// ---------------------------------------------------------------------------
// Packet receive handlers (called from overworld_multiplayer.cpp dispatcher)
// ---------------------------------------------------------------------------

// TEAMUP_BATTLE header accumulated — Player B (non-initiator) receives this.
// In the sync model, Player B has their own BSP from SetupBattleTrainer.
// We modify Player B's BSP to PP_AA, send ACK, and signal DeferEncountStart to release.
void overworldMPOnTeamUpBattleReceived(int32_t fromStation, uint8_t* data, int32_t size) {
    auto& tu = s_teamUpState;
    if (!tu.isTeamedUp || tu.partnerStation != fromStation) {
        MP_LOG("[TeamUp] Ignoring TEAMUP_BATTLE from non-partner station %d\n", fromStation);
        return;
    }

    // State guard: we should be in SYNC_MATCHED as non-initiator
    if (tu.syncPhase == SyncPhase::SYNC_WAITING) {
        // Race 4: TEAMUP_BATTLE arrived before we processed the match.
        // PIA ordering guarantees the SYNC_WAIT that caused the match was
        // processed first (same sender stream), so promote to SYNC_MATCHED.
        tu.syncPhase = SyncPhase::SYNC_MATCHED;
        tu.isInitiator = false;
        MP_LOG("[TeamUp] Promoted from SYNC_WAITING to SYNC_MATCHED on 0xC9\n");
    } else if (tu.syncPhase != SyncPhase::SYNC_MATCHED) {
        MP_LOG("[TeamUp] Ignoring TEAMUP_BATTLE: not in sync (phase=%d)\n",
                    (int)tu.syncPhase);
        return;
    }

    MP_LOG("[TeamUp] Received TEAMUP_BATTLE: type=%d trainer=%d — setting up Player B BSP\n",
                (int)tu.battleType, tu.battleTrainerID);

    // Validate BSP BEFORE sending ACK — if BSP is null, we can't set up
    // the team-up battle and must not tell Player A we're ready.
    auto* bsp = s_teamUpBSP;
    if (bsp == nullptr) {
        bsp = PlayerWork::get_battleSetupParam();
    }
    if (bsp == nullptr) {
        MP_LOG("[TeamUp] ERROR: Player B BSP is null — cannot ACK\n");
        tu.syncPhase = SyncPhase::SYNC_NONE;
        return;
    }

    tu.isInitiator = false;

    // Deserialize + validate the partner's party BEFORE sending the ACK. The ACK
    // commits Player A to the PP_AA battle; if we sent it and THEN failed to produce
    // a valid partner party, Player A would enter a 4-slot comm battle waiting on a
    // peer that never joins (soft-lock). Validate first; on failure send SYNC_CANCEL
    // so Player A cleanly falls back to a solo battle.
    auto* partnerParty = deserializeTeamUpParty(
        tu.partnerPartyBuf, tu.partnerPartyBufSize, tu.partnerPartyCount);
    if (partnerParty == nullptr) {
        MP_LOG("[TeamUp] ERROR: failed to deserialize partner party — cancelling sync\n");
        sendSyncCancel(fromStation, 1); // reason=1: partner data invalid
        tu.syncPhase = SyncPhase::SYNC_NONE;
        tu.partnerPartyValid = false;
        return;
    }
    tu.partnerPartyValid = true;

    // Partner party validated — now commit and tell Player A we're ready.
    overworldMPSendTeamUpBattleAck(fromStation);

    int32_t myStation = mpThisStationIndex();

    // Get our party (first TEAMUP_PARTY_LIMIT members)
    auto* myParty = PlayerWork::get_playerParty();
    if (myParty == nullptr) {
        MP_LOG("[TeamUp] ERROR: local party is null\n");
        tu.syncPhase = SyncPhase::SYNC_NONE;
        return;
    }

    // Reuse the existing party's class + raw ctor instead of newInstance() —
    // newInstance's lazy class-init null-derefs in the battle setup context.
    auto* myTrimmedParty = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)myParty->klass);
    _ILExternal::external<void>(0x2055D10, myTrimmedParty); // PokeParty::ctor()
    int32_t myCount = myParty->fields.m_memberCount;
    if (myCount > TEAMUP_PARTY_LIMIT) myCount = TEAMUP_PARTY_LIMIT;
    for (int i = 0; i < myCount; i++) {
        auto* src = myParty->GetMemberPointer(i);
        auto* dst = myTrimmedParty->GetMemberPointer(i);
        if (src && dst && src->fields.m_accessor && dst->fields.m_accessor) {
            uint8_t tmp[POKE_FULL_DATA_SIZE];
            src->fields.m_accessor->Serialize_FullData( tmp);
            dst->fields.m_accessor->Deserialize_FullData( tmp);
        }
    }
    myTrimmedParty->fields.m_memberCount = myCount;

    // Build MYSTATUS_COMM
    MYSTATUS_COMM::Object myStatus = {};
    myStatus.SetupFromPlayerWork();
    int32_t mystOff = 0;
    MYSTATUS_COMM::Object partnerStatus = deserializeTeamUpStatus(
        tu.partnerMystatusBuf, tu.partnerMystatusLen, mystOff);

    uint8_t regulation[4] = { 1, 6, 2, 0x07 };
    static uint8_t s_emptyCapsuleArrayB[32] = {};
    void* emptyCapsule = (void*)s_emptyCapsuleArrayB;

    // Save local animation setting
    auto* myConfig = PlayerWork::get_config();
    if (myConfig != nullptr) {
        s_savedWazaeffMode = myConfig->fields.wazaeff_mode;
        MP_LOG("[TeamUp] Player B: saved wazaeff_mode=%d\n", s_savedWazaeffMode);
    }

    // Capture Player B's own trainer party before SetupBattleComm destroys it.
    // BSP[1] currently holds Player B's trainer from Orig() — needed for dual-trainer mode.
    static uint8_t s_myTrainerBuf[6 * TEAMUP_POKE_FULL_DATA_SIZE];
    static int32_t s_myTrainerCount = 0;
    s_myTrainerCount = 0;

    auto* preBspFields = &bsp->instance()->fields;
    if (preBspFields->party != nullptr && preBspFields->party->max_length > 1) {
        auto* myTrainerParty = preBspFields->party->m_Items[1];
        if (myTrainerParty != nullptr) {
            s_myTrainerCount = myTrainerParty->fields.m_memberCount;
            if (s_myTrainerCount > 6) s_myTrainerCount = 6;
            for (int i = 0; i < s_myTrainerCount; i++) {
                auto* poke = myTrainerParty->GetMemberPointer(i);
                if (poke && poke->fields.m_accessor) {
                    poke->fields.m_accessor->Serialize_FullData(
                        &s_myTrainerBuf[i * TEAMUP_POKE_FULL_DATA_SIZE]);
                }
            }
            MP_LOG("[TeamUp] Player B: captured own trainer party (%d members)\n", s_myTrainerCount);
        }
    }

    // Save tr_data pointer — SetupBattleComm calls Clear() which nulls entries
    // in-place, but may replace the array pointer. We restore the pointer so
    // normalTrainer can write into the original (correctly-sized) array.
    void* savedTrDataB = preBspFields->tr_data;

    // SetupBattleComm: partner=slot0, us=slot1 (temp — rearranged below)
    Dpr::EncountTools::SetupBattleComm(bsp, 0, 1/*double*/, 1/*commPos*/, regulation,
        tu.partnerStation, partnerParty, &partnerStatus, emptyCapsule,
        myStation, myTrimmedParty, &myStatus, emptyCapsule,
        -1, nullptr, nullptr, nullptr,
        -1, nullptr, nullptr, nullptr,
        nullptr, nullptr, 0, 0);

    MP_LOG("[TeamUp] Player B: SetupBattleComm complete, setting up PP_AA...\n");

    // PP_AA layout: [partner(0), enemy1(1), us(2), enemy2(3)]
    // Rearrange: swap slot 1 ↔ slot 2 (us goes to slot 2)
    auto* bspFields = &bsp->instance()->fields;

    // Restore tr_data array pointer.  Keep commMode=1 (set by SetupBattleComm) so
    // InitializeCoroutine creates Net::Client and setSubProcForSetup routes to
    // setup_comm_double (which has PP_AA support).  commMode=0 would route to
    // setup_alone_double_multi, which doesn't handle PP_AA and crashes.
    // Rule MUST be DOUBLE for PP_AA — Player B's Orig used the script's rule (often SINGLE)
    // which would cause a crash when the battle engine allocates for 2 clients instead of 4.
    bspFields->tr_data = savedTrDataB;
    bspFields->rule = Dpr::Battle::Logic::BtlRule::BTL_RULE_DOUBLE;
    if (bspFields->party != nullptr && bspFields->party->max_length > 2) {
        auto* tmp = bspFields->party->m_Items[1];
        bspFields->party->m_Items[1] = bspFields->party->m_Items[2];
        bspFields->party->m_Items[2] = tmp;
    }
    if (bspFields->playerStatus != nullptr && bspFields->playerStatus->max_length > 2) {
        auto* tmp = bspFields->playerStatus->m_Items[1];
        bspFields->playerStatus->m_Items[1] = bspFields->playerStatus->m_Items[2];
        bspFields->playerStatus->m_Items[2] = tmp;
    }
    if (bspFields->stations != nullptr && bspFields->stations->max_length > 2) {
        auto tmp = bspFields->stations->m_Items[1];
        bspFields->stations->m_Items[1] = bspFields->stations->m_Items[2];
        bspFields->stations->m_Items[2] = tmp;
    }

    bspFields->multiMode = (tu.battleMultiMode == TEAMUP_MULTIMODE_SINGLE)
                           ? TEAMUP_MULTIMODE_SINGLE : TEAMUP_MULTIMODE_PP_AA;
    bspFields->commPos = 2;    // We (Player B) control slot 2
    // Keep competitor=COMM(3) from SetupBattleComm — this routes the server
    // through seq_EXIT_COMM which properly sends the battle result to comm
    // clients via SubProc_ExitCommTrainer. TRAINER(1) would route through
    // seq_EXIT_NPC which doesn't notify comm clients of the result.

    // Fill NPC enemy slots — three cases (same logic as Player A):
    // 1. Original double battle (enemyID1 != 0): two distinct NPC trainers
    // 2. Dual-trainer from mismatched player encounters
    // 3. Single trainer split across both slots
    if (tu.battleTrainerID2 != 0) {
        // Case 1: Original double battle — two NPC trainers (enemyID0 + enemyID1)
        normalTrainer(bsp, 1, tu.battleTrainerID);
        if (tu.trainerPartyValid && tu.trainerPartyCount > 0) {
            overwriteTrainerPartyFromBuffer(bsp, 1, tu);
        }
        normalTrainer(bsp, 3, tu.battleTrainerID2);
        // Sync the SECOND trainer's party from the initiator too (its party can be
        // per-save randomized, otherwise slot 3 desyncs between consoles).
        if (tu.trainerParty2Valid && tu.trainerParty2Count > 0) {
            overwriteTrainerPartyFromBufferEx(bsp, 3, tu.trainerParty2Buf,
                                              tu.trainerParty2BufSize, tu.trainerParty2Count);
        }
        MP_LOG("[TeamUp] Player B: DOUBLE BATTLE — slot1=%d(%d pokes), slot3=%d(%d pokes)\n",
                    tu.battleTrainerID, tu.trainerPartyCount, tu.battleTrainerID2, tu.trainerParty2Count);
    } else if (bspFields->multiMode == TEAMUP_MULTIMODE_SINGLE) {
        // Single-trainer PA_A2: client 1 covers BOTH enemy positions with its full
        // party (two mons out from one origin). No slot-3 client, no party split.
        normalTrainer(bsp, 1, tu.battleTrainerID);
        if (tu.trainerPartyValid && tu.trainerPartyCount > 0) {
            overwriteTrainerPartyFromBuffer(bsp, 1, tu);
        }
        owmpTeamUpLogMode6Topology(bsp);
        MP_LOG("[TeamUp] Player B: SINGLE-TRAINER PA_A2 — slot1=%d (%d pokes), slot3 empty\n",
                    tu.battleTrainerID, tu.trainerPartyCount);
    } else {
        bool dualTrainer = tu.trainerPartyCount < 3 &&
                           tu.syncTrainerID != tu.battleTrainerID &&
                           s_myTrainerCount > 0;

        if (dualTrainer) {
            // Case 2: Mismatched encounters — Player B's own trainer in slot 3
            normalTrainer(bsp, 1, tu.battleTrainerID);
            if (tu.trainerPartyValid && tu.trainerPartyCount > 0) {
                overwriteTrainerPartyFromBuffer(bsp, 1, tu);
            }
            normalTrainer(bsp, 3, tu.syncTrainerID);
            auto* party3 = bspFields->party->m_Items[3];
            if (party3 != nullptr) {
                for (int i = 0; i < s_myTrainerCount; i++) {
                    auto* poke = party3->GetMemberPointer(i);
                    if (poke && poke->fields.m_accessor) {
                        poke->fields.m_accessor->Deserialize_FullData(
                            &s_myTrainerBuf[i * TEAMUP_POKE_FULL_DATA_SIZE]);
                    }
                }
                party3->fields.m_memberCount = s_myTrainerCount;
            }
            MP_LOG("[TeamUp] Player B: DUAL TRAINER mode — slot1=%d(%d pokes), slot3=%d(%d pokes)\n",
                        tu.battleTrainerID, tu.trainerPartyCount, tu.syncTrainerID, s_myTrainerCount);
        } else {
            // Case 3: Single trainer split across both slots
            normalTrainer(bsp, 1, tu.battleTrainerID);
            int32_t slot3ID = (tu.syncTrainerID != tu.battleTrainerID && tu.syncTrainerID != 0)
                              ? tu.syncTrainerID : tu.battleTrainerID;
            normalTrainer(bsp, 3, slot3ID);
            if (tu.trainerPartyValid && tu.trainerPartyCount > 0) {
                overwriteTrainerPartyFromBuffer(bsp, 1, tu);
            }
            splitTrainerParty(bsp, 1, 3);
            if (slot3ID != tu.battleTrainerID) {
                MP_LOG("[TeamUp] Player B: DUAL TRAINER SPLIT — slot1=%d, slot3=%d\n",
                            tu.battleTrainerID, slot3ID);
            }
        }
    }

    // Fix stations for AI slots
    if (bspFields->stations != nullptr && bspFields->stations->max_length > 3) {
        bspFields->stations->m_Items[1] = -1;
        bspFields->stations->m_Items[3] = -1;
    }

    // Battle effect: TR_MULTI animation + trainer BGM
    if (bspFields->btlEffComponent != nullptr)
        setupTeamUpBattleEffect(bspFields->btlEffComponent, tu.battleEffectID);

    // Override arena — SetupBattleComm hardcodes Union Room arena 0x2b.
    // We must always override when battleArenaID was set by the trainer encounter.
    if (tu.battleArenaID >= 0) {
        void* fieldSit = bspFields->fieldSituation;
        if (fieldSit != nullptr) {
            void* bgComp = *(void**)((uintptr_t)fieldSit + 0x10);
            if (bgComp != nullptr) {
                _ILExternal::external<void>(0x188A6C0, bgComp, tu.battleArenaID, 0);
                MP_LOG("[TeamUp] Player B: arena override %d applied\n", tu.battleArenaID);
            } else {
                MP_LOG("[TeamUp] Player B: bgComp null, arena override skipped\n");
            }
        } else {
            MP_LOG("[TeamUp] Player B: fieldSituation null, arena override skipped\n");
        }
    } else {
        MP_LOG("[TeamUp] Player B: battleArenaID=%d, arena override skipped\n", tu.battleArenaID);
    }

    // Override human trainer colorIDs for MP — three data stores.
    // PP_AA layout for Player B: [partner(0), enemy1(1), us(2), enemy2(3)]
    // Guard partnerStation: -1 after Clear(), and a disconnect racing the exchange
    // could leave it out of range — skip the (cosmetic) color override rather than
    // indexing remotePlayers[] out of bounds.
    if (tu.partnerStation >= 0 && tu.partnerStation < OW_MP_MAX_PLAYERS)
    {
        int32_t localColor = getCustomSaveData()->playerColorVariation.playerColorID;
        auto& remote = getOverworldMPContext().remotePlayers[tu.partnerStation];
        MP_LOG("[TeamUp] Player B: color setup start (local=%d, partner=%d, hasCustom=%d)\n",
                    localColor, remote.colorId, (int)remote.hasCustomColors);
        // Player B PP_AA layout: local=slot2, partner=slot0. Status is re-fetched
        // from PlayerWork (the comm BSP), not this call's bsp param.
        auto* bspInst = PlayerWork::get_battleSetupParam();
        auto* statusArr = (bspInst != nullptr) ? bspInst->instance()->fields.playerStatus : nullptr;
        applyTeamUpBattleColors(bspFields->tr_data, statusArr, remote, localColor,
                                /*localSlot=*/2, /*partnerSlot=*/0);
    }
    {
        extern bool g_owmpBattleColorActive;
        g_owmpBattleColorActive = true;
    }

    // Enable CreateLocalClient hook
    s_teamUpCreateLocalClientActive = true;
    s_teamUpBSP = nullptr;

    MP_LOG("[TeamUp] Player B: PP_AA BSP complete (trainer=%d, effectID=%d, trainerPartyFromNet=%d)\n",
                tu.battleTrainerID, tu.battleEffectID, (int)tu.trainerPartyValid);

    // Signal DeferEncountStart to release on next tick
    tu.syncPhase = SyncPhase::SYNC_NONE;
}

// TEAMUP_BATTLE_ACK accumulated — partner responded with their party.
// Player A builds a full comm battle BSP using SetupBattleComm, replacing the
// trainer-battle BSP that Orig created. This gives us proper comm arrays (stations,
// playerStatus) that the trainer BSP doesn't have.
void overworldMPOnTeamUpBattleAckReceived(int32_t fromStation, uint8_t* data, int32_t size) {
    auto& tu = s_teamUpState;
    if (!tu.isTeamedUp || tu.partnerStation != fromStation) {
        MP_LOG("[TeamUp] Ignoring TEAMUP_BATTLE_ACK from non-partner station %d\n", fromStation);
        return;
    }

    // If we were demoted to joiner by the dual-trigger tie-breaker, ignore
    // the stale ACK — we're already entering battle as Player B
    if (!tu.isInitiator) {
        MP_LOG("[TeamUp] Ignoring TEAMUP_BATTLE_ACK: we are joiner, not initiator\n");
        return;
    }

    tu.partnerPartyValid = true;
    MP_LOG("[TeamUp] Received TEAMUP_BATTLE_ACK: partner party valid, %d members\n",
                tu.partnerPartyCount);

    auto* bsp = s_teamUpBSP;
    if (bsp == nullptr) {
        MP_LOG("[TeamUp] WARNING: s_teamUpBSP is null, cannot modify BSP\n");
        return;
    }

    // Deserialize partner party
    auto* partnerParty = deserializeTeamUpParty(
        tu.partnerPartyBuf, tu.partnerPartyBufSize, tu.partnerPartyCount);
    if (partnerParty == nullptr) {
        MP_LOG("[TeamUp] ERROR: failed to deserialize partner party for BSP\n");
        s_teamUpBSP = nullptr;
        tu.battlePending = false;
        return;
    }

    // Get our own party (trimmed to TEAMUP_PARTY_LIMIT)
    auto* myParty = PlayerWork::get_playerParty();
    if (myParty == nullptr) {
        MP_LOG("[TeamUp] ERROR: local party is null\n");
        s_teamUpBSP = nullptr;
        tu.battlePending = false;
        return;
    }

    // Reuse the existing party's class + raw ctor instead of newInstance() —
    // newInstance's lazy class-init null-derefs in the battle setup context.
    auto* myTrimmedParty = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)myParty->klass);
    _ILExternal::external<void>(0x2055D10, myTrimmedParty); // PokeParty::ctor()
    int32_t myCount = myParty->fields.m_memberCount;
    if (myCount > TEAMUP_PARTY_LIMIT) myCount = TEAMUP_PARTY_LIMIT;
    for (int i = 0; i < myCount; i++) {
        auto* src = myParty->GetMemberPointer(i);
        auto* dst = myTrimmedParty->GetMemberPointer(i);
        if (src && dst && src->fields.m_accessor && dst->fields.m_accessor) {
            uint8_t tmp[POKE_FULL_DATA_SIZE];
            src->fields.m_accessor->Serialize_FullData( tmp);
            dst->fields.m_accessor->Deserialize_FullData( tmp);
        }
    }
    myTrimmedParty->fields.m_memberCount = myCount;

    // Build MYSTATUS_COMM for both players
    MYSTATUS_COMM::Object myStatus = {};
    myStatus.SetupFromPlayerWork();

    int32_t mystOff = 0;
    MYSTATUS_COMM::Object partnerStatus = deserializeTeamUpStatus(
        tu.partnerMystatusBuf, tu.partnerMystatusLen, mystOff);

    uint8_t regulation[4] = { 1, 6, 2, 0x07 };
    static uint8_t s_emptyCapsuleArray2[32] = {};
    void* emptyCapsule = (void*)s_emptyCapsuleArray2;

    int32_t myStation = mpThisStationIndex();

    // Save local animation setting — SetupBattleComm (comm battle) will sync
    // from the server, overwriting the local player's preference
    auto* myConfig = PlayerWork::get_config();
    if (myConfig != nullptr) {
        s_savedWazaeffMode = myConfig->fields.wazaeff_mode;
        MP_LOG("[TeamUp] Player A: saved wazaeff_mode=%d\n", s_savedWazaeffMode);
    }

    // Save tr_data pointer — SetupBattleComm calls Clear() which nulls entries
    // in-place, but may replace the array pointer. We restore the pointer so
    // normalTrainer can write into the original (correctly-sized) array.
    auto* preSaveFields = &bsp->instance()->fields;
    void* savedTrData = preSaveFields->tr_data;

    // Call SetupBattleComm to allocate stations/playerStatus arrays.
    // This replaces the trainer BSP with a comm-battle BSP.
    Dpr::EncountTools::SetupBattleComm(bsp, 0, 1/*double*/, 0/*commPos*/, regulation,
        myStation, myTrimmedParty, &myStatus, emptyCapsule,             // slot 0: us
        tu.partnerStation, partnerParty, &partnerStatus, emptyCapsule,  // slot 1: partner
        -1, nullptr, nullptr, nullptr,                                   // slot 2: empty
        -1, nullptr, nullptr, nullptr,                                   // slot 3: empty
        nullptr, nullptr, 0, 0);

    MP_LOG("[TeamUp] Player A: SetupBattleComm complete, setting up PP_AA...\n");

    // PP_AA layout: [us(0), enemy1(1), partner(2), enemy2(3)]
    // SetupBattleComm put us=slot0, partner=slot1. Rearrange: move partner 1→2.
    auto* fields = &bsp->instance()->fields;

    // Restore tr_data array pointer.  Keep commMode=1 (set by SetupBattleComm) so
    // InitializeCoroutine creates Net::Client and setSubProcForSetup routes to
    // setup_comm_double (which has PP_AA support).  commMode=0 would route to
    // setup_alone_double_multi, which doesn't handle PP_AA and crashes.
    // Rule MUST be DOUBLE for PP_AA — explicitly set rather than restoring from savedRule
    // (savedRule is set to DOUBLE at SYNC_MATCHED for initiator, but explicit is safer).
    fields->tr_data = savedTrData;
    fields->rule = Dpr::Battle::Logic::BtlRule::BTL_RULE_DOUBLE;

    // Rearrange: swap slot 1 ↔ slot 2 (partner goes to slot 2, slot 1 freed for enemy)
    if (fields->party != nullptr && fields->party->max_length > 2) {
        auto* tmp = fields->party->m_Items[1];
        fields->party->m_Items[1] = fields->party->m_Items[2];
        fields->party->m_Items[2] = tmp;
    }
    if (fields->playerStatus != nullptr && fields->playerStatus->max_length > 2) {
        auto* tmp = fields->playerStatus->m_Items[1];
        fields->playerStatus->m_Items[1] = fields->playerStatus->m_Items[2];
        fields->playerStatus->m_Items[2] = tmp;
    }
    if (fields->stations != nullptr && fields->stations->max_length > 2) {
        auto tmp = fields->stations->m_Items[1];
        fields->stations->m_Items[1] = fields->stations->m_Items[2];
        fields->stations->m_Items[2] = tmp;
    }

    fields->multiMode = (tu.battleMultiMode == TEAMUP_MULTIMODE_SINGLE)
                        ? TEAMUP_MULTIMODE_SINGLE : TEAMUP_MULTIMODE_PP_AA;
    fields->commPos = 0;    // We (Player A) control slot 0
    // Keep competitor=COMM(3) from SetupBattleComm — this routes the server
    // through seq_EXIT_COMM which properly sends the battle result to comm
    // clients via SubProc_ExitCommTrainer. TRAINER(1) would route through
    // seq_EXIT_NPC which doesn't notify comm clients of the result.

    // Fill NPC enemy slots — three cases:
    // 1. Original double battle (enemyID1 != 0): two distinct NPC trainers
    // 2. Dual-trainer from mismatched player encounters: each player's trainer
    // 3. Single trainer split across both slots
    normalTrainer(bsp, 1, tu.battleTrainerID);
    int32_t myTrainerCount = 0;
    if (fields->party != nullptr && fields->party->max_length > 1 &&
        fields->party->m_Items[1] != nullptr) {
        myTrainerCount = fields->party->m_Items[1]->fields.m_memberCount;
    }

    if (tu.battleTrainerID2 != 0) {
        // Case 1: Original double battle — two NPC trainers (enemyID0 + enemyID1)
        normalTrainer(bsp, 3, tu.battleTrainerID2);
        MP_LOG("[TeamUp] Player A: DOUBLE BATTLE — slot1=%d(%d pokes), slot3=%d\n",
                    tu.battleTrainerID, myTrainerCount, tu.battleTrainerID2);
    } else if (fields->multiMode == TEAMUP_MULTIMODE_SINGLE) {
        // Single-trainer PA_A2: client 1 (already filled above via normalTrainer(1))
        // covers BOTH enemy positions with its full party. No slot-3 client, no split.
        owmpTeamUpLogMode6Topology(bsp);
        MP_LOG("[TeamUp] Player A: SINGLE-TRAINER PA_A2 — slot1=%d (%d pokes), slot3 empty\n",
                    tu.battleTrainerID, myTrainerCount);
    } else {
        bool dualTrainer = myTrainerCount < 3 &&
                           tu.partnerTrainerValid &&
                           tu.partnerTrainerID != tu.battleTrainerID;

        if (dualTrainer) {
            // Case 2: Mismatched encounters — Player B's trainer in slot 3
            normalTrainer(bsp, 3, tu.partnerTrainerID);
            auto* party3 = fields->party->m_Items[3];
            if (party3 != nullptr) {
                int32_t ptCount = tu.partnerTrainerCount;
                if (ptCount > 6) ptCount = 6;
                for (int i = 0; i < ptCount; i++) {
                    int32_t bufOff = i * TEAMUP_POKE_FULL_DATA_SIZE;
                    if (bufOff + TEAMUP_POKE_FULL_DATA_SIZE > tu.partnerTrainerBufSize) break;
                    auto* poke = party3->GetMemberPointer(i);
                    if (poke && poke->fields.m_accessor) {
                        poke->fields.m_accessor->Deserialize_FullData(
                            &tu.partnerTrainerBuf[bufOff]);
                    }
                }
                party3->fields.m_memberCount = ptCount;
            }
            MP_LOG("[TeamUp] Player A: DUAL TRAINER mode — slot1=%d(%d pokes), slot3=%d(%d pokes)\n",
                        tu.battleTrainerID, myTrainerCount, tu.partnerTrainerID, tu.partnerTrainerCount);
        } else {
            // Case 3: Single trainer split across both slots
            int32_t slot3ID = (tu.partnerTrainerID != tu.battleTrainerID && tu.partnerTrainerID != 0)
                              ? tu.partnerTrainerID : tu.battleTrainerID;
            normalTrainer(bsp, 3, slot3ID);
            splitTrainerParty(bsp, 1, 3);
            if (slot3ID != tu.battleTrainerID) {
                MP_LOG("[TeamUp] Player A: DUAL TRAINER SPLIT — slot1=%d, slot3=%d\n",
                            tu.battleTrainerID, slot3ID);
            }
        }
    }

    // normalTrainer may overwrite BSP stations for the filled slots.
    // Force stations[1] and [3] back to -1 so CheckReturnFromClient and
    // sendServerVersion correctly skip AI clients.
    if (fields->stations != nullptr && fields->stations->max_length > 3) {
        MP_LOG("[TeamUp] Player A: stations BEFORE fix: [%d, %d, %d, %d]\n",
                    fields->stations->m_Items[0], fields->stations->m_Items[1],
                    fields->stations->m_Items[2], fields->stations->m_Items[3]);
        fields->stations->m_Items[1] = -1;
        fields->stations->m_Items[3] = -1;
        MP_LOG("[TeamUp] Player A: stations AFTER fix: [%d, %d, %d, %d]\n",
                    fields->stations->m_Items[0], fields->stations->m_Items[1],
                    fields->stations->m_Items[2], fields->stations->m_Items[3]);
    }

    // btlEffComponent: use TR_MULTI (68) for 2v2 intro animation, but restore
    // the trainer's original BGM so gym leaders etc. keep their unique music.
    if (fields->btlEffComponent != nullptr)
        setupTeamUpBattleEffect(fields->btlEffComponent, tu.battleEffectID);
    MP_LOG("[TeamUp] Player A: PP_AA setup (trainer=%d/%d, effectID=%d)\n",
                tu.battleTrainerID, tu.battleTrainerID2, tu.battleEffectID);

    // Override arena — SetupBattleComm hardcodes Union Room arena 0x2b.
    // We must always override when battleArenaID was set by the trainer encounter.
    if (tu.battleArenaID >= 0) {
        void* fieldSit = fields->fieldSituation;
        if (fieldSit != nullptr) {
            void* bgComp = *(void**)((uintptr_t)fieldSit + 0x10);
            if (bgComp != nullptr) {
                _ILExternal::external<void>(0x188A6C0, bgComp, tu.battleArenaID, 0);
                MP_LOG("[TeamUp] Player A: arena override %d applied\n", tu.battleArenaID);
            } else {
                MP_LOG("[TeamUp] Player A: bgComp null, arena override skipped\n");
            }
        } else {
            MP_LOG("[TeamUp] Player A: fieldSituation null, arena override skipped\n");
        }
    } else {
        MP_LOG("[TeamUp] Player A: battleArenaID=%d, arena override skipped\n", tu.battleArenaID);
    }

    // Override human trainer colorIDs for MP — three data stores.
    // PP_AA layout for Player A: [us(0), enemy1(1), partner(2), enemy2(3)]
    // Guard partnerStation: -1 after Clear(), and a disconnect racing the exchange
    // could leave it out of range — skip the (cosmetic) color override rather than
    // indexing remotePlayers[] out of bounds.
    if (tu.partnerStation >= 0 && tu.partnerStation < OW_MP_MAX_PLAYERS)
    {
        int32_t localColor = getCustomSaveData()->playerColorVariation.playerColorID;
        auto& remote = getOverworldMPContext().remotePlayers[tu.partnerStation];
        MP_LOG("[TeamUp] Player A: color setup start (local=%d, partner=%d, hasCustom=%d)\n",
                    localColor, remote.colorId, (int)remote.hasCustomColors);
        // Player A PP_AA layout: local=slot0, partner=slot2. Status comes from this
        // BSP's own fields (already the comm BSP built by SetupBattleComm).
        applyTeamUpBattleColors(fields->tr_data, fields->playerStatus, remote, localColor,
                                /*localSlot=*/0, /*partnerSlot=*/2);
    }
    {
        extern bool g_owmpBattleColorActive;
        g_owmpBattleColorActive = true;
    }

    // Enable CreateLocalClient hook — the battle engine will call it during
    // setupseq_comm_create_server_client to create clients. We need local AI
    // clients for PP_AA slots 1,3 regardless of isRemoteServer.
    s_teamUpCreateLocalClientActive = true;

    s_teamUpBSP = nullptr;
    tu.battlePending = false;

    // Signal DeferEncountStart to release on next tick
    tu.syncPhase = SyncPhase::SYNC_NONE;

    // Track partner station for mid-battle disconnect detection
    overworldMPSetActiveBattlePartner(tu.partnerStation);

    if (!s_encountDeferred) {
        // Normal case (local/low-latency): EncountStart already called before ACK arrived.
        // Set in-battle scene flag now.
        overworldMPSetInBattleScene(true);
        MP_LOG("[TeamUp] Player A: PP_AA BSP complete (non-deferred)\n");
    } else {
        // Deferred case (internet): EncountStart was deferred, waiting for us to finish.
        // The tick function will detect syncPhase=SYNC_NONE and call EncountStart.
        MP_LOG("[TeamUp] Player A: PP_AA BSP complete (deferred — tick will release)\n");
    }
}

// (overworldMPSetupTeamUpBattle removed — Player B now uses sync-wait + DeferEncountStart.
//  BSP modification for Player B is done in overworldMPOnTeamUpBattleReceived.)

// (overworldMPSetupTeamUpBattle removed — Player B now gets BSP from their own event script.
//  BSP modification for Player B is done inline in overworldMPOnTeamUpBattleReceived.)

// ---------------------------------------------------------------------------
// Copy battle-modified party back to player's original party (EXP/HP/PP persistence)
// ---------------------------------------------------------------------------
static void copyBackBattleParty(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp) {
    if (bsp == nullptr) {
        MP_LOG("[TeamUp] copyBack: BSP is null!\n");
        return;
    }
    auto* fields = &bsp->instance()->fields;
    if (fields->party == nullptr) {
        MP_LOG("[TeamUp] copyBack: BSP party array is null!\n");
        return;
    }

    auto& tu = overworldMPGetTeamUpState();
    int32_t mySlot = tu.isInitiator ? 0 : 2;  // commPos (PP_AA: slot 0=Player A, 2=Player B)

    MP_LOG("[TeamUp] copyBack: isInitiator=%d mySlot=%d partyLen=%d\n",
                (int)tu.isInitiator, mySlot, fields->party->max_length);

    if ((uint64_t)mySlot >= fields->party->max_length) {
        MP_LOG("[TeamUp] copyBack: mySlot %d >= party max_length %d!\n",
                    mySlot, fields->party->max_length);
        return;
    }
    auto* battleParty = fields->party->m_Items[mySlot];
    if (battleParty == nullptr) {
        MP_LOG("[TeamUp] copyBack: battleParty at slot %d is null!\n", mySlot);
        return;
    }

    auto* origParty = PlayerWork::get_playerParty();
    if (origParty == nullptr) {
        MP_LOG("[TeamUp] copyBack: origParty is null!\n");
        return;
    }

    int32_t count = battleParty->fields.m_memberCount;
    if (count > TEAMUP_PARTY_LIMIT) count = TEAMUP_PARTY_LIMIT;

    MP_LOG("[TeamUp] copyBack: battleParty members=%d, origParty members=%d, copying %d\n",
                battleParty->fields.m_memberCount, origParty->fields.m_memberCount, count);

    for (int i = 0; i < count && i < origParty->fields.m_memberCount; i++) {
        auto* src = battleParty->GetMemberPointer(i);
        auto* dst = origParty->GetMemberPointer(i);
        if (src && dst && src->fields.m_accessor && dst->fields.m_accessor) {
            uint8_t tmp[POKE_FULL_DATA_SIZE];
            src->fields.m_accessor->Serialize_FullData( tmp);
            dst->fields.m_accessor->Deserialize_FullData( tmp);
        } else {
            MP_LOG("[TeamUp] copyBack: skip poke %d (src=%p dst=%p)\n", i, src, dst);
        }
    }
    MP_LOG("[TeamUp] Copied %d battle-modified Pokemon back to player party\n", count);
}

// ---------------------------------------------------------------------------
// Post-battle handler: party copy-back
// ---------------------------------------------------------------------------
// In the sync model, both players run their own event scripts, so both
// get proper post-battle rewards/whiteout/dialogue from the script.
// This handler only needs to fix up the party data (the battle engine
// writes only TEAMUP_PARTY_LIMIT members back, destroying slots 3+).
void overworldMPHandleTeamUpPostBattle() {
    auto& tu = overworldMPGetTeamUpState();
    if (tu.battleType != 1) return;  // not a trainer team-up battle

    // Apply deferred party restore — this runs AFTER vanilla FinalizeCoroutine
    // completes, so our data won't be overwritten by the BSP→PlayerWork copy.
    applyDeferredPartyRestore();

    MP_LOG("[TeamUp] Post-battle: party restore complete (isInitiator=%d, result=%d)\n",
                (int)tu.isInitiator, tu.battleResult);

    // On a team WIN, one player's Pokemon may all be fainted (the partner
    // finished the fight). Revive the first non-egg Pokemon to 1 HP to
    // prevent the whiteout sequence from triggering. On a LOSS, let the
    // normal Pokemon Center whiteout flow happen.
    if (tu.battleResult == 1) {
        auto* party = PlayerWork::get_playerParty();
        if (party != nullptr) {
            uint32_t count = party->fields.m_memberCount;
            bool allFainted = true;
            for (uint32_t i = 0; i < count; i++) {
                auto* poke = (Pml::PokePara::CoreParam::Object*)party->GetMemberPointer(i);
                if (poke == nullptr) continue;
                if (poke->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG)) continue;
                if (poke->GetHp() > 0) {
                    allFainted = false;
                    break;
                }
            }
            if (allFainted) {
                // Revive the first non-egg Pokemon to 1 HP
                for (uint32_t i = 0; i < count; i++) {
                    auto* poke = (Pml::PokePara::CoreParam::Object*)party->GetMemberPointer(i);
                    if (poke == nullptr) continue;
                    if (poke->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG)) continue;
                    poke->SetHp(1);
                    MP_LOG("[TeamUp] Revived party slot %u to 1 HP (all-fainted on team WIN)\n", i);
                    break;
                }
            }
        }
    }

    // Schedule a second-pass restore as a safety net — vanilla FinalizeCoroutine
    // may copy BSP.party[0] to PlayerWork AFTER our applyDeferredPartyRestore,
    // overwriting the correct data. Re-apply after a short delay.
    if (s_battleModPartyCount > 0 && !s_battleAbnormal) {
        s_postBattleRestoreTimer = 0.5f;
    }

    // Clear battle state
    tu.battleType = 0;
    tu.battleResult = -1;
    tu.battleGetMoney = 0;
}

// ---------------------------------------------------------------------------
// Post-battle cleanup
// ---------------------------------------------------------------------------
void overworldMPClearTeamUpBattleState() {
    s_teamUpCreateLocalClientActive = false;
    s_teamUpBSP = nullptr;
    s_expSecondPassDone = false;
    s_postBattleRestoreTimer = 0.0f; // Cancel second-pass restore — ClearState runs on
                                     // the same frame as the first restore, so the timer
                                     // would find s_battleModPartyCount stale if we zeroed it.
    // Defensive hygiene: if no encount is actively deferred, drop the cached
    // FieldManager pointer so it can't dangle into a later zone change. If one IS
    // deferred, leave it — the tick must still release it; clearing here would
    // strand the paused event script.
    if (!s_encountDeferred) {
        s_deferredFM = nullptr;
    }
    MP_LOG("[TeamUp] Battle state cleared\n");
}

// ---------------------------------------------------------------------------
// MainModule::GetEscapeMode hook
// ---------------------------------------------------------------------------
// With competitor=COMM(3), GetEscapeMode returns BTL_ESCAPE_MODE_CONFIRM (2)
// GetEscapeMode — merged into battle_escape_flag.cpp to avoid hook conflict.
// The team-up check is now part of MainModule_GetEscapeMode in that file.

// ---------------------------------------------------------------------------
// MainModule::IsExpSeqEnable hook
// ---------------------------------------------------------------------------
// With competitor=COMM(3), the vanilla IsExpSeqEnable returns false because
// it only allows EXP for competitor < 2 (WILD or TRAINER). Override to
// enable EXP gain in team-up trainer battles.
//
// IsExpSeqEnable @ 0x2034BF0

HOOK_DEFINE_TRAMPOLINE(TeamUpIsExpSeqEnable) {
    static bool Callback(void* mainModule, MethodInfo* mi) {
        if (overworldMPIsTeamedUp() && overworldMPGetTeamUpState().battleType == 1) {
            return true;
        }
        return Orig(mainModule, mi);
    }
};

// ---------------------------------------------------------------------------
// MainModule::storeBattleResult hook
// ---------------------------------------------------------------------------
// Abnormal ending override: If a disconnect, comm error, or escape occurred,
// force the result to LOSS so the event script doesn't mark the trainer as
// defeated. With competitor=COMM(3), the server's seq_EXIT_COMM properly
// sends the result to Player B via SubProc_ExitCommTrainer, so no manual
// result correction is needed.
//
// storeBattleResult @ 0x202D560
// BSP is at MainModule+0x10 (managed BATTLE_SETUP_PARAM object)
// EscapeInfo is at MainModule+0x110 → +0x98
// EscapeInfo::GetCount @ 0x1D11EB0

HOOK_DEFINE_TRAMPOLINE(TeamUpStoreBattleResult) {
    static void Callback(void* mainModule, MethodInfo* mi) {
        // Fast path: for non-team-up battles, just call Orig and return.
        // Minimizes our frame's presence on the stack to avoid .eh_frame
        // incompatibility crashes during the game's exception handling.
        if (!overworldMPIsTeamedUp()) {
            Orig(mainModule, mi);
            return;
        }

        auto& tu = overworldMPGetTeamUpState();
        if (tu.battleType != 1) {
            Orig(mainModule, mi);
            return;
        }

        // --- Team-up battle path ---
        // Pre-Orig diagnostics for team-up battles only
        uint32_t preJudgeResult = *(uint32_t*)((uintptr_t)mainModule + 0x8c);
        uint8_t myClientId = *(uint8_t*)((uintptr_t)mainModule + 0x94);

        auto* preBsp = *(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object**)
            ((uintptr_t)mainModule + 0x10);
        if (preBsp != nullptr) {
            auto* pf = &preBsp->instance()->fields;
            MP_LOG("[TeamUp] PRE-storeBattleResult: judgeResult(+0x8c)=%u myClientId(+0x94)=%u "
                        "BSP.commMode=%u BSP.commPos=%u BSP.result=%d BSP.getMoney=%d\n",
                        preJudgeResult, (unsigned)myClientId,
                        (unsigned)pf->commMode, (unsigned)pf->commPos,
                        pf->result, pf->getMoney);
        }

        Orig(mainModule, mi);

        MP_LOG("[TeamUp] storeBattleResult hook: teamed=%d battleType=%d isInitiator=%d\n",
                    (int)true, (int)tu.battleType, (int)tu.isInitiator);

        // Get BSP from MainModule+0x10
        auto* bsp = *(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object**)
            ((uintptr_t)mainModule + 0x10);
        if (bsp == nullptr) return;

        auto* fields = &bsp->instance()->fields;

        // --- Post-Orig diagnostics ---
        uint32_t postJudgeResult = *(uint32_t*)((uintptr_t)mainModule + 0x8c);
        MP_LOG("[TeamUp] POST-storeBattleResult: judgeResult(+0x8c)=%u BSP.result=%d BSP.getMoney=%d\n",
                    postJudgeResult, fields->result, fields->getMoney);

        // Check escape count via EscapeInfo (MainModule+0x110 → +0x98)
        int32_t escapeCount = 0;
        void* ctx = *(void**)((uintptr_t)mainModule + 0x110);
        if (ctx != nullptr) {
            void* escapeInfo = *(void**)((uintptr_t)ctx + 0x98);
            if (escapeInfo != nullptr) {
                escapeCount = _ILExternal::external<int32_t>(0x1D11EB0, escapeInfo);
            }
        }

        MP_LOG("[TeamUp] storeBattleResult: result=%d disconnect=%d commError=%d escapes=%d\n",
                    fields->result, (int)fields->isDisconnectOccur, fields->commError, escapeCount);

        // Override result to LOSS if battle ended abnormally
        bool abnormal = fields->isDisconnectOccur || fields->commError != 0 || escapeCount > 0;
        if (abnormal && fields->result != 0) {
            fields->result = 0;     // LOSS — trainer not defeated
            fields->getMoney = 0;   // no money gain from invalid battle
            MP_LOG("[TeamUp] Overrode trainer result to LOSS (abnormal ending)\n");
        }

        MP_LOG("[TeamUp] Final result: %d, getMoney: %d (isInitiator=%d)\n",
                    fields->result, fields->getMoney, (int)tu.isInitiator);

        // Cache final result for post-battle handler (Player B rewards/whiteout)
        tu.battleResult = fields->result;
        tu.battleGetMoney = fields->getMoney;

        // On LOSS: switch competitor from COMM(3) back to TRAINER(1) so the
        // exit sequence uses seq_EXIT_NPC instead of seq_EXIT_COMM. This
        // ensures the event script sees the loss result and triggers whiteout.
        // On WIN, keep COMM(3) so the comm sync works properly.
        if (fields->result == 0) {
            fields->competitor = (Dpr::Battle::Logic::BtlCompetitor)1; // BTL_COMPETITOR_TRAINER
            MP_LOG("[TeamUp] LOSS: switched competitor to TRAINER for proper exit sequence\n");
        }

        // NPC-parity: on a WIN, credit the win money to the wallet. Our setSubProc remap
        // makes the NPC exit UI DISPLAY "got money", but the actual wallet credit (done
        // by the trainer's overworld script for a normal battle) is skipped on the comm
        // path — so add it once here. GetBonusMoney @0x202DB50 is the value CalcWinMoney
        // (which we ungate for team-up) produced and the exit screen shows.
        if (!abnormal && fields->result != 0 && s_teamUpWinMoney > 0) {
            int32_t cur = PlayerWork::GetMoney();
            PlayerWork::SetMoney(cur + s_teamUpWinMoney);
            MP_LOG("[TeamUp] NPC-parity: credited %d win money (wallet %d -> %d)\n",
                        s_teamUpWinMoney, cur, cur + s_teamUpWinMoney);
            s_teamUpWinMoney = 0;
        }

        // On loss, skip the deferred restore — vanilla TRAINER exit handles whiteout.
        // Only save battle-modified data on win, where we need EXP/damage persistence.

        // Save state for deferred restore in post-battle handler.
        // We CANNOT restore PlayerWork's party here because the vanilla
        // FinalizeCoroutine continues AFTER storeBattleResult and copies
        // BSP party[0] to PlayerWork — overwriting our restore.
        // Instead, save the battle-modified data to a buffer and let
        // overworldMPHandleTeamUpPostBattle apply it after finalization.
        s_battleAbnormal = abnormal;
        s_battleModPartyCount = 0;

        if (!abnormal) {
            // Persist exp/level/moves/evolution on BOTH win and loss (pull the
            // exp-modified party from GetSrcParty via the POKECON conversion). The
            // only difference is HP: on a WIN we keep the battle HP (with a wipe->1HP
            // rescue if the team won while this player fainted); on a LOSS we heal the
            // whole party to full (the pokémon-center restore) while still keeping the
            // exp/level/move gains.
            //
            // Capture NOW as the guaranteed baseline (GetSrcParty is valid here),
            // then flag a top-up at Clear. The final KO's exp may not have landed in
            // the POKECON yet, so Clear re-runs the conversion once it has — but if
            // that late pass fails it can't wipe this one (see saveBattleModifiedParty).
            s_captureSrcParty   = nullptr;  // fresh resolve for this battle
            saveBattleModifiedParty(mainModule, myClientId, /*isWin=*/fields->result != 0);
            s_pendingExpCapture = true;
            s_captureMainModule = mainModule;
            s_captureClientId   = myClientId;
            s_captureIsWin      = fields->result != 0;
        } else {
            s_pendingExpCapture = false;
        }
        MP_LOG("[TeamUp] storeBattleResult: capture deferred to Clear (abnormal=%d, pending=%d)\n",
                    (int)abnormal, (int)s_pendingExpCapture);

        // Restore local animation setting — comm battles sync from the server,
        // which may have a different wazaeff_mode than us
        if (s_savedWazaeffMode >= 0) {
            auto* localConfig = PlayerWork::get_config();
            if (localConfig != nullptr) {
                MP_LOG("[TeamUp] Restoring wazaeff_mode: %d -> %d\n",
                            localConfig->fields.wazaeff_mode, s_savedWazaeffMode);
                localConfig->fields.wazaeff_mode = s_savedWazaeffMode;
            }
            s_savedWazaeffMode = -1;
        }
    }
};

// ---------------------------------------------------------------------------
// NPC-parity: run the NPC trainer-defeat exit (demo + money) on a team-up WIN
// ---------------------------------------------------------------------------
// BTL_CLIENT::setSubProc(client, stateId:byte, out*) @ 0x1F4F4F0 sets up the client's
// exit sub-proc from a state id: 0x19 = comm-trainer exit (bare PvP result sync, no
// defeat demo/money), 0x1a = NPC exit (SubProc_UI_ExitForNPC: sad-face defeat demo +
// "got money"). Team-up LOSSES already switch competitor->TRAINER so the server sends
// 0x1a; only the team-up WIN takes 0x19. Remap that 0x19 -> 0x1a so a co-op win gets
// the full NPC trainer-defeat presentation. (Both players were present for the win, so
// the comm result sync the 0x19 path does is redundant here.)
HOOK_DEFINE_TRAMPOLINE(TeamUpClientSetSubProc) {
    static uint64_t Callback(void* client, uint8_t stateId, uint8_t* out, MethodInfo* mi) {
        if (stateId == 0x19 && overworldMPIsTeamedUp() &&
            overworldMPGetTeamUpState().battleType == 1) {
            MP_LOG("[TeamUp] NPC-parity: remap comm-exit (0x19) -> NPC-exit (0x1a)\n");
            stateId = 0x1a;
        }
        return Orig(client, stateId, out, mi);
    }
};

// NPC-parity: win money. calc::CalcWinMoney @ 0x1F76980 returns 0 unless competitor is
// TRAINER(1) (`if (*(int*)(param1+0x10) != 1) return 0;`). Team-up runs as COMM(3), so
// the win money is 0. Present TRAINER for just this one call so the game's own trainer
// win-money formula (last enemy level * trainer gold param * 4) runs; the value flows to
// MainModule bonus money, which the now-active NPC exit UI adds + shows ("got money").
HOOK_DEFINE_TRAMPOLINE(TeamUpCalcWinMoney) {
    static int32_t Callback(void* param1, MethodInfo* mi) {
        if (param1 != nullptr && overworldMPIsTeamedUp() &&
            overworldMPGetTeamUpState().battleType == 1) {
            int32_t* competitor = (int32_t*)((uintptr_t)param1 + 0x10);
            int32_t saved = *competitor;
            *competitor = 1;  // TRAINER, only across this call
            int32_t money = Orig(param1, mi);
            *competitor = saved;
            s_teamUpWinMoney = money;  // credited on the win in storeBattleResult
            MP_LOG("[TeamUp] NPC-parity: CalcWinMoney -> %d (was gated to 0)\n", money);
            return money;
        }
        return Orig(param1, mi);
    }
};

// ---------------------------------------------------------------------------
// BATTLE_SETUP_PARAM::Clear hook — late exp capture
// ---------------------------------------------------------------------------
// FinalizeCoroutine calls Clear at its very end (after StopBGM), right before it
// wipes the BSP and one state later reads PlayerWork for the evolution demo. This
// is the latest safe point at which the battle POKECON still holds the FULL,
// final exp — including the last KO's chunk that hadn't landed at storeBattleResult
// time. When a team-up battle has flagged a pending capture, run the conversion +
// PlayerWork writeback HERE (before Orig clears anything). Gated by the pending
// flag so the identical Clear calls at battle SETUP and in solo battles are ignored.
//
// Clear @ 0x1AC3AC0
HOOK_DEFINE_TRAMPOLINE(TeamUpBattleSetupClear) {
    static void Callback(void* bsp, MethodInfo* mi) {
        // Belt-and-suspenders: only act inside an active team-up battle, so a leaked
        // s_pendingExpCapture can never run the capture against a solo battle's teardown.
        if (s_pendingExpCapture && overworldMPIsTeamedUp() &&
            overworldMPGetTeamUpState().battleType == 1) {
            s_pendingExpCapture = false;
            saveBattleModifiedParty(s_captureMainModule, s_captureClientId, s_captureIsWin);
        }
        Orig(bsp, mi);
    }
};

// ---------------------------------------------------------------------------
// Net.Client::NotifyNetworkError hook
// ---------------------------------------------------------------------------
// External callers (PIA callbacks, session management) call NotifyNetworkError
// to set comm error bits: commError |= 1 << errorCode.
// In team-up PvE battles, these errors are non-fatal — the battle data sync
// still works (proven by successful move selection), but the error check
// aborts the battle after events like KO + EXP distribution.
//
// Fix: during team-up battles, log but suppress external error notifications.
// The battle continues running; if a real desync occurs, storeBattleResult
// will detect it via result comparison.
//
// NotifyNetworkError @ 0x2036E30
// Signature: void NotifyNetworkError(Net.Client* this, uint32_t errorCode)

HOOK_DEFINE_TRAMPOLINE(TeamUpNotifyNetworkError) {
    static void Callback(void* netClient, uint32_t errorCode) {
        if (overworldMPIsTeamedUp()) {
            MP_LOG("[TeamUp] BtlNet NotifyNetworkError(%d) [bit 0x%x] — suppressed\n",
                        errorCode, 1u << (errorCode & 0x1f));
            return;  // suppress — non-fatal in team-up PvE
        }
        Orig(netClient, errorCode);
    }
};

// ---------------------------------------------------------------------------
// Net.Client::HasErrorOccured hook
// ---------------------------------------------------------------------------
// The battle engine polls HasErrorOccured each frame. In team-up battles,
// mask out non-fatal BtlNet error bits so the battle doesn't abort.
//
// Error bit meanings:
//   0x02: station leave      0x04: station disconnect  0x08: session event 7
//   0x10: session event 8    0x20: session event 9     0x40: server version timeout
//   0x80: external error 7   0x100: serialize error    0x200: PIA send failure
//   0x400: received error signal from partner
//
// For team-up: suppress all non-fatal bits (0x80, 0x200, 0x400).
// Fatal bits (0x02=leave, 0x04=disconnect) are kept — these mean the PIA
// session is truly dead and the battle can't continue.
//
// HasErrorOccured @ 0x2032F00
// Signature: bool HasErrorOccured(Net.Client* this, uint32_t mask)

static constexpr uint32_t TEAMUP_NONFATAL_ERRORS = 0x080 | 0x100 | 0x200 | 0x400;

HOOK_DEFINE_TRAMPOLINE(TeamUpHasErrorOccured) {
    static bool Callback(void* netClient, uint32_t mask) {
        if (overworldMPIsTeamedUp()) {
            // Read actual error bits from Net.Client+0x60
            uint32_t errors = *(uint32_t*)((uintptr_t)netClient + 0x60);
            if (errors != 0) {
                // Clear non-fatal bits before the check
                uint32_t fatal = errors & ~TEAMUP_NONFATAL_ERRORS;
                if (fatal != errors) {
                    // Some non-fatal bits were set — clear them from the actual field
                    *(uint32_t*)((uintptr_t)netClient + 0x60) = fatal;
                    static int32_t s_suppressCount = 0;
                    s_suppressCount++;
                    if (s_suppressCount <= 10 || s_suppressCount % 200 == 0) {
                        MP_LOG("[TeamUp] BtlNet HasErrorOccured: cleared non-fatal 0x%x, remaining 0x%x (call #%d)\n",
                                    errors & TEAMUP_NONFATAL_ERRORS, fatal, s_suppressCount);
                    }
                }
            }
        }
        return Orig(netClient, mask);
    }
};

// ---------------------------------------------------------------------------
// Net.Client::get_ErrorKindBits hook
// ---------------------------------------------------------------------------
// The battle engine reads error bits via this property getter (+0x60).
// Many code paths write to +0x60 DIRECTLY (OnReceivePacketEx sets 0x400,
// sendToClientCore sets 0x200), bypassing NotifyNetworkError entirely.
// This hook is the read-side counterpart: mask out non-fatal bits so the
// battle engine never sees them, regardless of how they were written.
//
// Also clears the masked bits from the actual field to prevent accumulation
// and stop the client from sending error signals to the partner.
//
// get_ErrorKindBits @ 0x203D4D0
// Signature: uint32_t get_ErrorKindBits(Net.Client* this)

HOOK_DEFINE_TRAMPOLINE(TeamUpGetErrorKindBits) {
    static uint32_t Callback(void* netClient) {
        uint32_t bits = Orig(netClient);
        if (overworldMPIsTeamedUp() && (bits & TEAMUP_NONFATAL_ERRORS)) {
            uint32_t suppressed = bits & TEAMUP_NONFATAL_ERRORS;
            bits &= ~TEAMUP_NONFATAL_ERRORS;
            // Clear in the actual field to prevent accumulation and
            // stop the client from sending error signals to partner
            *(uint32_t*)((uintptr_t)netClient + 0x60) = bits;
            static int32_t s_errBitsLogCount = 0;
            s_errBitsLogCount++;
            if (s_errBitsLogCount <= 10 || s_errBitsLogCount % 500 == 0) {
                MP_LOG("[TeamUp] get_ErrorKindBits: masked 0x%x, returning 0x%x (call #%d)\n",
                            suppressed, bits, s_errBitsLogCount);
            }
        }
        return bits;
    }
};

// ---------------------------------------------------------------------------
// Net.Client::sendToClientCore hook
// ---------------------------------------------------------------------------
// sendToClientCore (0x203F970) is the function that actually sends battle data
// to each client via PIA.  In PP_AA team-up, clients 1 and 3 are local AI
// (enemy trainer slots) and don't have real PIA stations.
//
// Previous approach of checking BSP stations for -1 was unreliable because
// normalTrainer() can overwrite station values when filling slot data.
//
// Fix: simply skip clients 1 and 3 by index when in a team-up battle.
// BSP stations are also forced to -1 in the ACK handler / Player B setup
// so that CheckReturnFromClient and sendServerVersion handle them correctly.
//
// sendToClientCore @ 0x203F970
// Signature: int64_t sendToClientCore(Net.Client* this, uint64_t clientIdx)

HOOK_DEFINE_TRAMPOLINE(TeamUpSendToClientCore) {
    static int64_t Callback(void* netClient, uint64_t clientIdx) {
        if (overworldMPIsTeamedUp()) {
            uint32_t idx = (uint32_t)(clientIdx & 0xFF);

            // In PP_AA team-up, clients 1 and 3 are AI (enemy trainer slots).
            // They don't have real PIA stations — skip sending entirely.
            // Previous approach of reading BSP stations was unreliable because
            // normalTrainer() can overwrite station values for filled slots.
            if (idx == 1 || idx == 3) {
                static int32_t s_skipCount = 0;
                s_skipCount++;
                if (s_skipCount <= 10 || s_skipCount % 1000 == 0) {
                    MP_LOG("[TeamUp] sendToClientCore: skip AI client %d (#%d)\n",
                                idx, s_skipCount);
                }
                return 0;  // success — pretend send completed
            }
        }
        return Orig(netClient, clientIdx);
    }
};

// ---------------------------------------------------------------------------
// BattleRule::IsClientAi_Double hook  (PA_A2 single-trainer team-up)
// ---------------------------------------------------------------------------
// IsClientAi_Double @0x1881750 (param_1, multiMode, slot) -> bool.
// For multiMode=6 (PA_A2) vanilla shares the [0,1,1,1] mask (BattleRule.txt:442-451),
// which marks client 2 — our comm ally — as AI. Override so ONLY client 1 (the enemy
// trainer covering both enemy positions) is AI; clients 0 and 2 are the two humans.
// Gated on team-up + mode 6, so vanilla PA_A2 battles (story doubles with an AI
// partner) are untouched. This is the highest-risk correction: a wrong bit here makes
// the ally AI-controlled on one peer and silently desyncs the battle.
HOOK_DEFINE_TRAMPOLINE(TeamUpIsClientAiDouble) {
    static uint64_t Callback(uint32_t param_1, uint8_t multiMode, uint32_t slot) {
        if (overworldMPIsTeamedUp() && multiMode == TEAMUP_MULTIMODE_SINGLE) {
            return (slot == 1) ? 1 : 0;
        }
        return Orig(param_1, multiMode, slot);
    }
};

// ---------------------------------------------------------------------------
// MainModule::setup_comm_double hook  (PA_A2 enemy-client registration)
// ---------------------------------------------------------------------------
// setup_comm_double @0x2032B60. At step 0 the vanilla init registers the AI enemy
// trainer client list only when multiMode==2 (MainModule.txt:4036-4048): it sets the
// count at +0x9d and writes clients [1,3] into the byte list at *(+0xa0)+0x20/0x21.
// For PA_A2 that block is skipped, so the lone enemy trainer (client 1) never gets
// registered and its party would never be stored/synced. Inject count=1, list=[1]
// after step 0. Defensive: only writes if the list object is allocated with capacity.
HOOK_DEFINE_TRAMPOLINE(TeamUpSetupCommDouble) {
    static uint64_t Callback(void* param_1, int32_t* param_2) {
        int32_t step = (param_2 != nullptr) ? *param_2 : -1;
        uint64_t r = Orig(param_1, param_2);
        if (step == 0 && overworldMPIsTeamedUp()) {
            uintptr_t p = (uintptr_t)param_1;
            void* inner = *(void**)(p + 0x10);
            if (inner != nullptr &&
                *(uint8_t*)((uintptr_t)inner + 0x39) == TEAMUP_MULTIMODE_SINGLE) {
                uintptr_t listObj = *(uintptr_t*)(p + 0xa0);
                if (listObj != 0) {
                    uint32_t cap = *(uint32_t*)(listObj + 0x18);
                    if (cap >= 1) {
                        *(uint8_t*)(p + 0x9d)       = 1;  // one enemy trainer client
                        *(uint8_t*)(listObj + 0x20) = 1;  // list[0] = client 1
                        MP_LOG("[TeamUp] PA_A2 inject: enemy client list=[1] count=1 (cap=%d)\n",
                                    (int)cap);
                    } else {
                        MP_LOG("[TeamUp] PA_A2 inject SKIP: list cap=%d < 1\n", (int)cap);
                    }
                } else {
                    MP_LOG("[TeamUp] PA_A2 inject SKIP: +0xa0 enemy list is null\n");
                }
            }
        }
        return r;
    }
};

// ---------------------------------------------------------------------------
// BattleSequenceSystem::BTL_SEQ_FUNC_DEF_MessageDispStd hook  (PA_A2 intro greeting)
// ---------------------------------------------------------------------------
// The team-up TR_MULTI intro uses the TWO-trainer greeting label (0x211 / 0x212,
// "... challenged by {A} and {B}!"), whose builder unconditionally registers two
// enemy trainer names via GetEnemyClientID(0) and GetEnemyClientID(1). In our PA_A2
// single-trainer battle both resolve to client 1, so the one trainer is named twice
// ("... William and William"). Rewrite the label to the vanilla single-trainer
// greeting (0x20f / 0x210), which registers exactly one name. Gated on team-up +
// PA_A2 so genuine two-trainer doubles (two distinct enemy clients) keep "and".
//
// BTL_SEQ_FUNC_DEF_MessageDispStd @0x1743DF0
//   (param_1, param_2, BattleViewSystem* param_3, cmdHolder param_4)
//   msgId = *(int*)( *(void**)(param_4 + 0x30) + 0x30 )   (Ghidra plVar18[6])
HOOK_DEFINE_TRAMPOLINE(TeamUpSingleTrainerGreeting) {
    static void Callback(void* param_1, void* param_2, void* param_3, void* param_4) {
        if (overworldMPIsTeamedUp() &&
            overworldMPGetTeamUpState().battleMultiMode == TEAMUP_MULTIMODE_SINGLE) {
            void* cmd = *(void**)((uintptr_t)param_4 + 0x30);
            if (cmd != nullptr) {
                int32_t* msgId = (int32_t*)((uintptr_t)cmd + 0x30);
                if (*msgId == 0x211) *msgId = 0x20f;        // "A and B" -> "A"
                else if (*msgId == 0x212) *msgId = 0x210;   // (same-species variant)
            }
        }
        Orig(param_1, param_2, param_3, param_4);
    }
};

// ---------------------------------------------------------------------------
// MainModule::setupseq_comm_start_server hook
// ---------------------------------------------------------------------------
// The vanilla game blocks commMode=1 (wireless) + multiMode=2 (PP_AA) at step 3
// of the comm server startup sequence. This combination never occurs in vanilla,
// but our team-up feature uses it for cooperative PvE battles over local wireless.
//
// The check at step 3 (RVA 0x2033AF0, case 3):
//   if (multiMode == 2 && commMode == 1) { errorFlag = 1; return 1; }
//
// Fix: when step==3 and it's our PP_AA+wireless combination, skip the check
// by advancing the step counter to 4, which causes the sequence to complete
// normally (default case returns 1 without setting the error flag).
//
// setupseq_comm_start_server @ 0x2033AF0

HOOK_DEFINE_TRAMPOLINE(TeamUpCommStartServer) {
    static uint64_t Callback(void* mainModule, int32_t* stepPtr) {
        if (*stepPtr == 3) {
            int32_t multiMode = *(int32_t*)((uintptr_t)mainModule + 0x70);
            if (multiMode == 2 || multiMode == TEAMUP_MULTIMODE_SINGLE) {
                void* battleEnv = *(void**)((uintptr_t)mainModule + 0x10);
                if (battleEnv != nullptr) {
                    uint8_t commMode = *(uint8_t*)((uintptr_t)battleEnv + 0x38);
                    if (commMode == 1) {
                        MP_LOG("[TeamUp] Bypassing step 3 PP_AA+wireless check\n");
                        *stepPtr = 4;
                        return 0;
                    }
                }
            }
        }
        return Orig(mainModule, stepPtr);
    }
};

// ---------------------------------------------------------------------------
// ServerClientFactory::CreateLocalClient hook
// ---------------------------------------------------------------------------
// The vanilla game never uses PP_AA (multiMode=2) with commMode!=0 (wireless).
// In wireless comm battles, CreateLocalClient only creates local AI clients
// when isRemoteServer=true (the server side). The not-server side skips them,
// and CreateRemoteClients then creates wireless adapters for AI slots with
// station=-1 → crash.
//
// Fix: after Orig returns 0 (no client), check if this is a PP_AA + wireless
// AI slot, and if so force-create a local AI client (type=1, commMode forced
// to 0 inside CreateClientObject).
//
// ServerClientFactory::CreateLocalClient @ 0x1F1CF50
// ServerClientFactory::CreateClientObject @ 0x1F1D010
// BattleRule::IsClientAi @ 0x1881600
//
// Input struct layout (from setupCommon_CreateServerClient):
//   +0x10: battleRuleObj (long, managed BattleRule*)
//   +0x18: btlRule (uint32, enum value)
//   +0x1c: commMode (uint8)
//   +0x1d: multiMode/stationType (uint8)
//   +0x24: isReplay (bool/char)
//   +0x28: commPos (uint32)
//   +0x2c: isRemoteServer (bool/char)

HOOK_DEFINE_TRAMPOLINE(TeamUpCreateLocalClient) {
    static long Callback(void** inputHolder, void* param_2, uint64_t slot, MethodInfo* mi) {
        long result = Orig(inputHolder, param_2, slot, mi);
        if (result != 0) return result;  // Orig created a client — done

        // Only intervene for team-up PP_AA + wireless
        if (!s_teamUpCreateLocalClientActive) return 0;

        uintptr_t input = (uintptr_t)*inputHolder;
        uint8_t commMode  = *(uint8_t*)(input + 0x1c);
        uint8_t multiMode = *(uint8_t*)(input + 0x1d);

        // Not a team-up comm double (PP_AA or PA_A2 single-trainer over wireless)
        if (commMode == 0 || (multiMode != 2 && multiMode != TEAMUP_MULTIMODE_SINGLE))
            return 0;

        // Double battles have at most 4 clients (0-3). SetupBattleComm may allocate
        // 5 slots (commRule+3), but slot 4 is not a real participant.
        // IsClientAi creates a bool[4] — calling it with slot>=4 would OOB crash.
        // (PA_A2 uses clients 0,1,2 only; the hooked IsClientAi returns AI for slot 1.)
        if (slot >= 4) return 0;

        // Check if this slot is AI via BattleRule::IsClientAi
        uint32_t btlRule = *(uint32_t*)(input + 0x18);
        bool isAi = _ILExternal::external<bool>(0x1881600,
            btlRule, commMode, multiMode, (uint32_t)(slot & 0xFFFFFFFF), (MethodInfo*)nullptr);
        if (!isAi) return 0;  // Human slot — let CreateRemoteClients handle it

        // Force-create local AI client (type=1 → commMode forced to 0 inside CreateClientObject)
        // CreateClientObject(Input** inputHolder, MethodInfo* unused, uint slot, char clientType)
        long aiClient = _ILExternal::external<long>(0x1F1D010,
            inputHolder, (void*)nullptr, (uint32_t)(slot & 0xFFFFFFFF), (int8_t)1);

        MP_LOG("[TeamUp] Force-created local AI client for slot %d (PP_AA + wireless)\n",
                    (int)(slot & 0xFF));
        return aiClient;
    }
};

// ---------------------------------------------------------------------------
// DeferEncountStart hook — FieldManager::EncountStart @ 0x0179f330
// ---------------------------------------------------------------------------
// The event script calls SetupBattleTrainer and EncountStart on the same
// frame. Over the internet, the ACK (with partner party data) takes 100-300ms
// to arrive. Without deferral, the battle engine reads the unmodified BSP,
// causing crashes, wrong Pokemon, and corruption.
//
// Fix: when a team-up battle is pending and the ACK hasn't arrived yet,
// don't call EncountStart. Instead, pause the event script by setting
// _updateType=2, and wait. The tick function checks each frame; when the
// ACK arrives and the BSP is fully modified, it calls the real EncountStart.

HOOK_DEFINE_TRAMPOLINE(DeferEncountStart) {
    static void Callback(FieldManager::Object* fm, int32_t type,
                         int32_t trainerid1, int32_t trainerid2, MethodInfo* mi)
    {
        auto& tu = s_teamUpState;

        // Defer EncountStart for BOTH players during any sync phase,
        // OR when s_teamUpEncountPending is set (SetupBattleTrainer in progress,
        // syncPhase not yet set — catches EncountStart called from within
        // SetupBattleTrainer::Orig before our post-Orig code runs).
        bool shouldDefer = (tu.isTeamedUp && tu.syncPhase != SyncPhase::SYNC_NONE)
                           || s_teamUpEncountPending;

        if (shouldDefer) {
            // If already deferred (second EncountStart call), just update args
            if (s_encountDeferred) {
                MP_LOG("[TeamUp] EncountStart called again while already deferred — updating args\n");
                s_deferredFM = fm;
                s_deferredType = type;
                s_deferredTrainer1 = trainerid1;
                s_deferredTrainer2 = trainerid2;
                return;
            }

            // Defer — save args for later
            s_deferredFM = fm;
            s_deferredType = type;
            s_deferredTrainer1 = trainerid1;
            s_deferredTrainer2 = trainerid2;
            s_encountDeferred = true;
            s_deferredTimer = DEFERRED_ENCOUNT_TIMEOUT;

            // Freeze the event script WITHOUT triggering battle transition.
            // _updateType=2 is "encounter mode" — the game's FieldManager::Update sees
            // _updateType==2 and initiates the battle scene load. We need a value that
            // stops the event script dispatcher from advancing but is NOT recognized as
            // encounter mode. An unhandled value (99) causes the update loop's switch to
            // fall through to default (no-op), effectively freezing the script.
            // releaseDeferredEncount will reset to 0 before calling the real EncountStart.
            fm->fields._updateType = 99;

            MP_LOG("[TeamUp] Deferring EncountStart (syncPhase=%d, pending=%d, timeout=%.1fs, _updateType=99)\n",
                        (int)tu.syncPhase, (int)s_teamUpEncountPending, DEFERRED_ENCOUNT_TIMEOUT);
            return;
        }

        MP_LOG("[TeamUp] EncountStart NOT deferred (teamed=%d, phase=%d, pending=%d)\n",
                    (int)tu.isTeamedUp, (int)tu.syncPhase, (int)s_teamUpEncountPending);
        Orig(fm, type, trainerid1, trainerid2, mi);
    }
};

// ---------------------------------------------------------------------------
// Release deferred EncountStart — call the real EncountStart and enter battle
// ---------------------------------------------------------------------------
static void releaseDeferredEncount(bool isTeamUp) {
    s_encountDeferred = false;
    s_teamUpEncountPending = false; // clear pre-Orig guard

    auto& tu = s_teamUpState;
    tu.bypassTrainerFlag = false; // no longer needed after sync resolves

    if (s_deferredFM != nullptr) {
        // Reset _updateType so EncountStart can set it properly.
        // Single-threaded: event system can't run between our reset
        // and the Orig call, so there's no gap where it would resume.
        s_deferredFM->fields._updateType = 0;
    }

    // Dismiss the waiting dialogue box
    if (s_syncMessageShown) {
        hideMsgWindowCustomText();
        s_syncMessageShown = false;
    }

    // Call the real EncountStart — either with fully modified PP_AA BSP,
    // or with the original trainer BSP as fallback.
    DeferEncountStart::Orig(s_deferredFM, s_deferredType,
                            s_deferredTrainer1, s_deferredTrainer2, nullptr);

    if (isTeamUp) {
        // Track partner station for mid-battle disconnect detection
        overworldMPSetActiveBattlePartner(s_teamUpState.partnerStation);
        overworldMPSetInBattleScene(true);
    }

    MP_LOG("[TeamUp] Deferred EncountStart released (teamUp=%d)\n", (int)isTeamUp);
}

// ---------------------------------------------------------------------------
// MsgWindow puppeting helpers — use the game's dialogue box frame directly
// ---------------------------------------------------------------------------
// Walk the component chain to get the TextMeshProUGUI inside MsgWindow:
//   MsgWindowManager (singleton) → MsgWindow (+0x38) → WindowMessage (+0x28)
//   → MsgTextContainer (+0x18) → WindowMsgText[] (+0x20, Il2CppArray)
//   → WindowMsgText[0] → TextMeshProUGUI (+0x18)
//
// MsgWindow::OnUpdate (0x1FAE530) returns immediately when MsgState == 0.
// Since we never touch MsgWindowDataModel, MsgState stays 0 — the update
// loop is a no-op. We just show the frame, set text, and hide when done.

static void* getMsgWindowTMP() {
    // Return cached pointer if available
    if (s_cachedMsgWindowTMP != nullptr) return s_cachedMsgWindowTMP;

    auto* mgr = Dpr::MsgWindow::MsgWindowManager::get_Instance();
    if (mgr == nullptr) {
        MP_LOG("[TeamUp] getMsgWindowTMP: MsgWindowManager instance is null\n");
        return nullptr;
    }

    void* msgWindow = mgr->get_MsgWindow();
    if (msgWindow == nullptr) {
        MP_LOG("[TeamUp] getMsgWindowTMP: MsgWindow is null\n");
        return nullptr;
    }

    // WindowMessage at MsgWindow+0x28
    void* windowMessage = *(void**)((uintptr_t)msgWindow + 0x28);
    if (windowMessage == nullptr) {
        MP_LOG("[TeamUp] getMsgWindowTMP: WindowMessage is null\n");
        return nullptr;
    }

    // MsgTextContainer at WindowMessage+0x18
    void* msgTextContainer = *(void**)((uintptr_t)windowMessage + 0x18);
    if (msgTextContainer == nullptr) {
        MP_LOG("[TeamUp] getMsgWindowTMP: MsgTextContainer is null\n");
        return nullptr;
    }

    // WindowMsgText[] (Il2CppArray) at MsgTextContainer+0x20
    void* textArray = *(void**)((uintptr_t)msgTextContainer + 0x20);
    if (textArray == nullptr) {
        MP_LOG("[TeamUp] getMsgWindowTMP: WindowMsgText array is null\n");
        return nullptr;
    }

    // Check array bounds: max_length at array+0x18
    uint64_t maxLength = *(uint64_t*)((uintptr_t)textArray + 0x18);
    if (maxLength == 0) {
        MP_LOG("[TeamUp] getMsgWindowTMP: WindowMsgText array is empty\n");
        return nullptr;
    }

    // WindowMsgText[0] at array+0x20 (m_Items start)
    void* windowMsgText = *(void**)((uintptr_t)textArray + 0x20);
    if (windowMsgText == nullptr) {
        MP_LOG("[TeamUp] getMsgWindowTMP: WindowMsgText[0] is null\n");
        return nullptr;
    }

    // TextMeshProUGUI at WindowMsgText+0x18
    void* tmp = *(void**)((uintptr_t)windowMsgText + 0x18);
    if (tmp == nullptr) {
        MP_LOG("[TeamUp] getMsgWindowTMP: TextMeshProUGUI is null\n");
        return nullptr;
    }

    s_cachedMsgWindowTMP = tmp;
    MP_LOG("[TeamUp] getMsgWindowTMP: cached TMP at %p\n", tmp);
    return tmp;
}

// Show/hide MsgWindow using WindowAnimator's DoPlayImmediateAnimation.
// This sets CanvasGroup alpha AND Transform scale in one call, bypassing
// DOTween entirely. DoPlayImmediateAnimation(animator, 1) = open (alpha=1, scale=1).
// DoPlayImmediateAnimation(animator, 0) = close (alpha=0, scale=0).
// RVAs: MsgWindow::SetActive 0x1FAC740, DoPlayImmediateAnimation 0x1DDC840
static void setMsgWindowActive(bool active) {
    auto* mgr = Dpr::MsgWindow::MsgWindowManager::get_Instance();
    if (mgr == nullptr) return;

    void* msgWindow = mgr->get_MsgWindow();
    if (msgWindow == nullptr) return;

    // SetActive on the GameObject first
    _ILExternal::external<void>(0x1FAC740, msgWindow, active);

    // WindowAnimator at MsgWindow+0x38
    void* windowAnimator = *(void**)((uintptr_t)msgWindow + 0x38);
    if (windowAnimator == nullptr) return;

    // DoPlayImmediateAnimation: sets alpha, scale, and state instantly
    _ILExternal::external<void>(0x1DDC840, windowAnimator, active);
    MP_LOG("[TeamUp] setMsgWindowActive(%d)\n", (int)active);
}

static void showMsgWindowCustomText(const char* text) {
    void* tmp = getMsgWindowTMP();
    if (tmp == nullptr) {
        // Fallback: use old DisplayMessage approach if MsgWindow chain unavailable
        MP_LOG("[TeamUp] showMsgWindowCustomText: MsgWindow chain failed, using DisplayMessage fallback\n");
        if (isFieldCanvasReady()) {
            auto* msgStr = System::String::Create(text);
            if (msgStr != nullptr) {
                FieldCanvas::Debug_ShowDisplayMessage(msgStr);
            }
        }
        return;
    }

    // Activate the MsgWindow frame
    setMsgWindowActive(true);

    // Set text directly on the TMP component (same RVA as AreaNameWindow hook)
    auto* str = System::String::Create(text);
    if (str != nullptr) {
        _ILExternal::external<void>(0x01e94520, tmp, str);
    }
}

static void hideMsgWindowCustomText() {
    setMsgWindowActive(false);
    // Invalidate TMP cache — window may be recreated by scene transitions
    s_cachedMsgWindowTMP = nullptr;
}

// ---------------------------------------------------------------------------
// Show the "Waiting for partner..." message using the MsgWindow dialogue box.
// The MsgWindow frame provides the styled background — no TMP <mark> tags needed.
// ---------------------------------------------------------------------------
// Replace {0}, {1}, ... placeholders in a template string with provided values.
static char* formatMPTemplate(char* outBuf, size_t outSize,
                               const char* tmpl,
                               const char* const* values, int valueCount) {
    char* dst = outBuf;
    char* end = outBuf + outSize - 1;
    const char* src = tmpl;
    while (*src && dst < end) {
        if (src[0] == '{' && src[1] >= '0' && src[1] <= '9' && src[2] == '}') {
            int idx = src[1] - '0';
            if (idx < valueCount && values[idx] != nullptr) {
                size_t len = strlen(values[idx]);
                if (dst + len > end) len = end - dst;
                memcpy(dst, values[idx], len);
                dst += len;
            }
            src += 3;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
    return outBuf;
}

// Load a string from the ss_multiplayer message file as C string, with fallback.
static const char* getTUMPMessageCStr(const char* label, const char* fallback) {
    auto* mgr = Dpr::Message::MessageManager::get_Instance();
    if (mgr == nullptr) return fallback;
    auto* str = mgr->GetSimpleMessage(
        System::String::Create("ss_multiplayer"),
        System::String::Create(label));
    if (str == nullptr || System::String::IsNullOrEmpty(str)) return fallback;
    static char s_tuMsgBuf[256];
    auto cstr = str->asCString();
    strncpy(s_tuMsgBuf, cstr.c_str(), sizeof(s_tuMsgBuf) - 1);
    s_tuMsgBuf[sizeof(s_tuMsgBuf) - 1] = '\0';
    // YAML single-quoted strings store \n as literal backslash+n — convert to real newlines
    for (char* p = s_tuMsgBuf; *p; p++) {
        if (p[0] == '\\' && p[1] == 'n') {
            p[0] = '\n';
            memmove(p + 1, p + 2, strlen(p + 2) + 1);
        }
    }
    return s_tuMsgBuf;
}

static void showSyncWaitMessage() {
    auto& tu = s_teamUpState;
    auto& ctx = getOverworldMPContext();
    int32_t ps = tu.partnerStation;

    // Resolve the partner name into a LOCAL buffer. getTUMPMessageCStr returns a
    // shared static buffer, so the message-template loads below would otherwise
    // overwrite the name if it came from getTUMPMessageCStr (the default-name case).
    const char* defaultName = getTUMPMessageCStr("SS_mp_DefaultName", "Player");
    char partnerName[52];
    if (ps >= 0 && ps < OW_MP_MAX_PLAYERS && ctx.remotePlayers[ps].playerNameSet) {
        strncpy(partnerName, ctx.remotePlayers[ps].playerNameBuf, sizeof(partnerName) - 1);
    } else {
        strncpy(partnerName, defaultName, sizeof(partnerName) - 1);
    }
    partnerName[sizeof(partnerName) - 1] = '\0';

    // Load translatable message templates from the ss_multiplayer bundle, falling
    // back to English if the label isn't present. TMP tags: <size=80%> shrinks text
    // to fit the window; <line-height=70%> tightens the (generous) default gap.
    char msgBuf[256];
    // MSBT has proper Name tags (ev-as). Tags: {0}=partner name
    if (tu.partnerSyncMismatch) {
        char tmpBuf[200];
        const char* vals[] = { partnerName };
        formatMPTemplate(tmpBuf, sizeof(tmpBuf),
            getTUMPMessageCStr("SS_mp_SyncReady", "{0} is ready!\nY: Synchronize  B: Battle solo"),
            vals, 1);
        snprintf(msgBuf, sizeof(msgBuf),
            "<size=80%%><line-height=70%%>%s", tmpBuf);
    } else {
        char tmpBuf[200];
        const char* vals[] = { partnerName };
        formatMPTemplate(tmpBuf, sizeof(tmpBuf),
            getTUMPMessageCStr("SS_mp_SyncWaiting", "Waiting on {0}...\nPress B to battle solo"),
            vals, 1);
        snprintf(msgBuf, sizeof(msgBuf),
            "<size=80%%><line-height=70%%>%s", tmpBuf);
    }

    showMsgWindowCustomText(msgBuf);
    s_syncMessageShown = true;
    MP_LOG("[TeamUp] Showing sync message for partner '%s'\n", partnerName);
}

// Called each frame from the main MP tick
void overworldMPTickDeferredEncount() {
    if (!s_encountDeferred) return;

    auto& tu = s_teamUpState;

    // -----------------------------------------------------------------------
    // Phase: SYNC_NONE — sync completed or cancelled, release deferral
    // -----------------------------------------------------------------------
    if (tu.syncPhase == SyncPhase::SYNC_NONE) {
        bool wasTeamUp = tu.partnerPartyValid && tu.isTeamedUp;
        MP_LOG("[TeamUp] Tick: syncPhase=NONE — releasing (%s)\n",
                    wasTeamUp ? "team-up BSP" : "solo fallback");
        // Consume the flag so it can't leak into the NEXT encounter. partnerPartyValid
        // is only valid for the exchange that just produced a PP_AA BSP; if it survived,
        // a later cancelled encounter (while still teamed up) would be mis-released as
        // team-up against an unmodified solo BSP.
        tu.partnerPartyValid = false;
        releaseDeferredEncount(wasTeamUp);
        return;
    }

    // -----------------------------------------------------------------------
    // Phase: SYNC_WAITING — waiting for partner to trigger same trainer
    // No timeout — player presses A to cancel and go solo whenever they want.
    // -----------------------------------------------------------------------
    if (tu.syncPhase == SyncPhase::SYNC_WAITING) {
        // Cancel sync: read controller state directly (InputHelper's stateful
        // isHold*/isPress* API depends on the debug ImGui loop — see
        // readNpadStateDirect's doc comment).
        nn::hid::NpadBaseState padState = InputHelper::readNpadStateDirect();
        bool cancelPressed = padState.mButtons.isBitSet(nn::hid::NpadButton::B);
        bool syncPressed   = padState.mButtons.isBitSet(nn::hid::NpadButton::Y);

        // Diagnostic: log every 2 seconds to confirm tick is running and show button state
        static float s_syncWaitLogTimer = 0.0f;
        s_syncWaitLogTimer += 0.01666667f;
        if (s_syncWaitLogTimer >= 2.0f) {
            s_syncWaitLogTimer = 0.0f;
            // Cast NpadBaseState to get raw button bits for logging
            uint64_t rawButtons = 0;
            memcpy(&rawButtons, &padState.mButtons, sizeof(rawButtons));
            MP_LOG("[TeamUp] SYNC_WAITING tick: buttons=0x%llx cancel=%d\n",
                        (unsigned long long)rawButtons, (int)cancelPressed);
        }

        if (cancelPressed) {
            s_syncWaitLogTimer = 0.0f;
            MP_LOG("[TeamUp] Tick: cancel button pressed — going solo\n");
            overworldMPCancelSyncAndGoSolo();
            // syncPhase is now SYNC_NONE, next tick will release
            return;
        }

        // Y-to-synchronize: partner sent SYNC_WAIT with different trainer ID
        if (tu.partnerSyncMismatch && syncPressed) {
            MP_LOG("[TeamUp] Y pressed — force-synchronizing with mismatched trainer\n");
            tu.partnerSyncReceived = true;
            tu.partnerSyncMismatch = false;
            s_syncMessageShown = false; // refresh message on next tick
        }

        // Show "Waiting for partner..." message on first tick.
        // We're now AFTER FieldManager::Update Orig() returns — safe to call
        // FieldCanvas. The AreaNameWindowSetNameText hook bypasses label lookup.
        if (!s_syncMessageShown) {
            showSyncWaitMessage();
        }

        // Check if partner sync matched (set by onSyncWaitReceived)
        if (tu.partnerSyncReceived) {
            tu.partnerSyncReceived = false;
            tu.syncPhase = SyncPhase::SYNC_MATCHED;
            s_deferredTimer = DEFERRED_ENCOUNT_TIMEOUT; // start exchange timeout
            // Area name window dismissed by releaseDeferredEncount when _updateType returns to 0

            // Determine initiator: lower station ID
            int32_t myStation = mpThisStationIndex();
            tu.isInitiator = (myStation < tu.partnerStation);

            MP_LOG("[TeamUp] SYNC_MATCHED! initiator=%d (myStation=%d, partner=%d)\n",
                        (int)tu.isInitiator, myStation, tu.partnerStation);

            // Force BSP to DOUBLE now that both players are confirmed
            auto* bsp = s_teamUpBSP;
            if (bsp != nullptr) {
                bsp->instance()->fields.rule = Dpr::Battle::Logic::BtlRule::BTL_RULE_DOUBLE;
                MP_LOG("[TeamUp] Forced BSP to DOUBLE\n");
            }

            if (tu.isInitiator) {
                // Send TEAMUP_BATTLE (0xC9) with our party
                overworldMPSendTeamUpBattleTrainer(tu.partnerStation,
                    tu.battleTrainerID, tu.battleArenaID, tu.battleWeatherType);
                tu.battlePending = true;
            }
            // Non-initiator: waits for 0xC9 from partner (handled in receive handler)
            return;
        }

        return;
    }

    // -----------------------------------------------------------------------
    // Phase: SYNC_MATCHED — party exchange in progress (5s timeout)
    // -----------------------------------------------------------------------
    // The handlers (overworldMPOnTeamUpBattleAckReceived for initiator,
    // overworldMPOnTeamUpBattleReceived for non-initiator) will set
    // syncPhase=SYNC_NONE when BSP modification is complete.
    s_deferredTimer -= 0.01666667f; // ~1 frame at 60fps
    if (s_deferredTimer <= 0.0f) {
        MP_LOG("[TeamUp] SYNC_MATCHED exchange TIMED OUT (%.1fs) — going solo\n",
                    DEFERRED_ENCOUNT_TIMEOUT);

        tu.syncPhase = SyncPhase::SYNC_NONE;
        tu.battlePending = false;
        tu.partnerPartyValid = false;
        tu.bypassTrainerFlag = false;
        releaseDeferredEncount(false);
        return;
    }
}

// ---------------------------------------------------------------------------
// EXP distribution fix for team-up battles
// ---------------------------------------------------------------------------
// In PP_AA comm battles, Section_CheckExpGet::Execute only calculates and
// distributes EXP for mainModule+0x94 (myClientId = commPos). On the server
// (Player A, commPos=0), this means only party[0] gets EXP. Player B's party
// at slot 2 is treated like an NPC partner and receives +0 EXP.
//
// Fix: after the first pass completes, temporarily change myClientId to the
// partner's client ID and re-run the EXP flow. DeadRec exp-checked flags
// are bypassed during the second pass so the same dead enemies are re-processed.
//
// Section_CheckExpGet::Execute @ 0x21AF510
// DeadRec::GetExpCheckedFlag @ 0x1D10CB0
// DeadRec::SetExpCheckedFlag @ 0x1D10B60

static bool s_expSecondPass = false;
// True when the vanilla first pass marked a newly-dead enemy as exp-checked THIS
// frame — i.e. a real KO just happened. This is the correct "new KO" trigger for
// the partner's second pass, independent of whether the LOCAL client's pokemon
// participated (firstResult, the old trigger, was 0 for a partner-only KO — e.g.
// the final enemy killed by the partner alone — so the partner never got that exp).
static bool s_expNewEnemyChecked = false;
// s_expSecondPassDone declared near top of file with other globals

// Only the JOINER (non-initiator) needs the own-tracking override: it receives the
// host's synced "checked" flags, which make its local calc skip the final KO for its
// own pokemon. The initiator/host is authoritative — its real flags are correct, so we
// leave it on vanilla (overriding it there corrupts its exp calc).
static inline bool owmpJoinerExpFixActive() {
    return overworldMPIsTeamedUp() &&
           overworldMPGetTeamUpState().battleType == 1 &&
           !overworldMPGetTeamUpState().isInitiator;
}

// Capture the pokecon the client-side executor applies exp into. This is the object
// that actually levels up (matches the on-screen gauge/stat-summary), and on the joiner
// it's NOT the server-synced pokecon we used to read. AddExp signature:
//   AddExp(ServerCommandExecutor* exec, uint32 pokeID, int32 expAmount)
//   pokecon = *(*(exec + 0x18) + 0x10)
HOOK_DEFINE_TRAMPOLINE(TeamUpExecutorAddExp) {
    static void Callback(void* exec, uint32_t pokeID, int32_t expAmount, MethodInfo* mi) {
        if (overworldMPIsTeamedUp() && overworldMPGetTeamUpState().battleType == 1 && exec != nullptr) {
            void* a = *(void**)((uintptr_t)exec + 0x18);
            if (a != nullptr) s_expApplyPokecon = *(void**)((uintptr_t)a + 0x10);
        }
        Orig(exec, pokeID, expAmount, mi);
    }
};

HOOK_DEFINE_TRAMPOLINE(TeamUpGetExpCheckedFlag) {
    static bool Callback(void* deadRec, uint8_t side, uint8_t idx, void* mi) {
        if (s_expSecondPass) return false;  // partner (second) pass: re-check everything
        // Joiner local (first) pass: trust OUR OWN per-record set, not the host-synced
        // checked flag, so we commit every KO our own pokemon earned exactly once
        // (false = not yet awarded → process). calcExp queries with side=0 + the
        // dead-RECORD index; index by the real (side,idx) with bounds so any other
        // (side,idx) the game might use falls back to the real flag instead of aliasing.
        if (owmpJoinerExpFixActive() && side < 4 && idx < 6) {
            return s_localAwarded[side][idx];
        }
        return Orig(deadRec, side, idx, mi);
    }
};

HOOK_DEFINE_TRAMPOLINE(TeamUpSetExpCheckedFlag) {
    static void Callback(void* deadRec, uint8_t side, uint8_t idx, void* mi) {
        if (s_expSecondPass) return;  // Don't mark during second pass
        // Mark this dead record committed in our own set so the joiner won't re-award it
        // (prevents a runaway when we ignore the host's synced flag above).
        if (owmpJoinerExpFixActive() && idx < 6) {
            s_localAwarded[0][idx] = true;
        }
        s_expNewEnemyChecked = true;  // first pass processed a newly-dead record → real KO
        Orig(deadRec, side, idx, mi);
    }
};

HOOK_DEFINE_TRAMPOLINE(TeamUpCheckExpGetExecute) {
    static void Callback(long param_1, long param_2) {
        // Reentrancy guard — the second Orig call below re-enters this hook
        if (s_expSecondPass) {
            Orig(param_1, param_2);
            return;
        }

        s_expNewEnemyChecked = false;   // cleared before the first pass; set by SetExpCheckedFlag if a KO is processed
        Orig(param_1, param_2);

        if (!overworldMPIsTeamedUp()) return;
        auto& tu = overworldMPGetTeamUpState();
        if (tu.battleType != 1) return;

        long mainModule = *(long*)(param_1 + 0x10);
        if (mainModule == 0) return;

        // Save first pass result
        uint8_t firstResult = *(uint8_t*)(param_2 + 0x10);

        // Reset done-flag when a NEW enemy was actually KO'd this frame — detected via
        // SetExpCheckedFlag, not firstResult. firstResult only reflects whether the
        // LOCAL client's pokemon gained exp; when the partner alone lands a KO (very
        // common for the final enemy) firstResult is 0 yet a real KO happened, and the
        // old check skipped the partner's second pass so they lost that KO's exp.
        if (s_expNewEnemyChecked) {
            s_expSecondPassDone = false;
        }

        // Skip if second pass already ran for this batch of dead enemies.
        // Without this guard, calcExp sees dead enemies as unchecked every frame
        // (GetExpCheckedFlag forced false) and Op_AddExp queues duplicate EXP infinitely.
        if (s_expSecondPassDone) return;

        uint8_t myClientId = *(uint8_t*)(mainModule + 0x94);
        // Partner is at the other ally slot in PP_AA: 0↔2
        uint8_t partnerClientId = (myClientId == 0) ? 2 : 0;

        MP_LOG("[TeamUp] EXP fix: running second pass for partner client %d (my=%d)\n",
                    (int)partnerClientId, (int)myClientId);

        // Temporarily switch to partner's client ID
        *(uint8_t*)(mainModule + 0x94) = partnerClientId;
        s_expSecondPass = true;

        Orig(param_1, param_2);

        // Restore
        s_expSecondPass = false;
        *(uint8_t*)(mainModule + 0x94) = myClientId;
        s_expSecondPassDone = true;

        // Combine results (either pass produced EXP → flag set)
        *(uint8_t*)(param_2 + 0x10) = firstResult | *(uint8_t*)(param_2 + 0x10);

        MP_LOG("[TeamUp] EXP fix: second pass complete (result=%d|%d)\n",
                    (int)firstResult, (int)*(uint8_t*)(param_2 + 0x10));
    }
};

// ---------------------------------------------------------------------------
// Hook installation
// ---------------------------------------------------------------------------
void exl_team_up_main() {
    s_teamUpState.Clear();

    // Hook EncountStart to defer battle transition until ACK arrives
    DeferEncountStart::InstallAtOffset(0x0179f330);

    // Hook CreateLocalClient to fix PP_AA + wireless AI slot creation
    TeamUpCreateLocalClient::InstallAtOffset(0x1F1CF50);

    // Hook setupseq_comm_start_server to bypass step 3 PP_AA+wireless block
    TeamUpCommStartServer::InstallAtOffset(0x2033AF0);
    // PA_A2 single-trainer team-up mode (one enemy trainer covers both enemy positions)
    TeamUpIsClientAiDouble::InstallAtOffset(0x1881750);
    TeamUpSetupCommDouble::InstallAtOffset(0x2032B60);
    TeamUpSingleTrainerGreeting::InstallAtOffset(0x1743DF0);

    // GetEscapeMode hook merged into battle_escape_flag.cpp

    // Hook IsExpSeqEnable to allow EXP gain with competitor=COMM(3)
    TeamUpIsExpSeqEnable::InstallAtOffset(0x2034BF0);

    // Hook storeBattleResult to prevent trainer defeat on disconnect/escape
    TeamUpStoreBattleResult::InstallAtOffset(0x202D560);
    TeamUpBattleSetupClear::InstallAtOffset(0x1AC3AC0);
    TeamUpExecutorAddExp::InstallAtOffset(0x1F1DC90);
    TeamUpClientSetSubProc::InstallAtOffset(0x1F4F4F0);
    TeamUpCalcWinMoney::InstallAtOffset(0x1F76980);

    // Hook BtlNet error handling to suppress non-fatal errors in team-up
    TeamUpNotifyNetworkError::InstallAtOffset(0x2036E30);
    TeamUpHasErrorOccured::InstallAtOffset(0x2032F00);
    TeamUpGetErrorKindBits::InstallAtOffset(0x203D4D0);

    // Hook sendToClientCore to skip sends for AI clients (station=-1 in BSP)
    TeamUpSendToClientCore::InstallAtOffset(0x203F970);

    // Hook AreaNameWindow::SetNameText to support custom text display
    // (bypasses message label lookup when s_useCustomAreaText is set)
    AreaNameWindowSetNameText::InstallAtOffset(0x21209C0);

    // Hook EXP distribution to also process partner's party in PP_AA
    TeamUpCheckExpGetExecute::InstallAtOffset(0x21AF510);
    TeamUpGetExpCheckedFlag::InstallAtOffset(0x1D10CB0);
    TeamUpSetExpCheckedFlag::InstallAtOffset(0x1D10B60);

    MP_LOG("[TeamUp] Hooks installed\n");
}
