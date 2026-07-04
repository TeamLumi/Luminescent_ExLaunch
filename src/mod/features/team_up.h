#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/Pml/PokeParty.h"

// ---------------------------------------------------------------------------
// Team-Up: Cooperative overworld multiplayer (double battles)
// ---------------------------------------------------------------------------
// When two players "Team Up", trainer encounters become double battles
// with PP_AA (multiMode=2): both humans control one Pokemon each (slots 0,2),
// the other two slots (1,3) are AI enemies from the same trainer.
//
// Sync model: Player A triggers a trainer → enters SYNC_WAITING → Player B
// independently triggers the same trainer → match detected → party exchange
// → both enter PP_AA double battle → both event scripts resume after battle.

// PokePara full data size (same as overworld_multiplayer.cpp)
static constexpr int32_t TEAMUP_POKE_FULL_DATA_SIZE = 344;

// Max Pokemon per player in team-up battles (first 3 of party)
static constexpr int32_t TEAMUP_PARTY_LIMIT = 3;

// Sync phase for the trainer rendezvous state machine
enum class SyncPhase : int32_t {
    SYNC_NONE     = 0,  // Not in sync — normal operation
    SYNC_WAITING  = 1,  // Sent SYNC_WAIT, waiting for partner to trigger same trainer
    SYNC_MATCHED  = 2,  // Both triggered same trainer, doing party exchange
};

// Team-up state tracked on each console
struct TeamUpState {
    bool isTeamedUp;
    int32_t partnerStation;         // remote player's station index

    // Partner data (received fresh before each battle via TEAMUP_BATTLE / ACK)
    uint8_t partnerPartyBuf[6 * TEAMUP_POKE_FULL_DATA_SIZE]; // max 6 pokes serialized
    int32_t partnerPartyBufSize;
    int32_t partnerPartyCount;
    bool partnerPartyValid;         // set true when battle party ACK received

    // Partner MYSTATUS_COMM (received in TEAMUP_BATTLE / ACK)
    uint8_t partnerMystatusBuf[128];
    int32_t partnerMystatusLen;

    // Trainer party data (received from initiator in TEAMUP_BATTLE)
    // Used so both players fight the same enemy Pokemon regardless of per-save randomization.
    uint8_t trainerPartyBuf[6 * TEAMUP_POKE_FULL_DATA_SIZE]; // max 6 trainer pokes serialized
    int32_t trainerPartyBufSize;
    int32_t trainerPartyCount;
    bool trainerPartyValid;         // set true when all trainer POKE sub-packets received

    // Second real trainer's party (slot 3) — genuine two-trainer double battles only
    // (battleTrainerID2 != 0). Sent by the initiator in TEAMUP_BATTLE alongside
    // trainerPartyBuf so both consoles fight the same second trainer's Pokemon.
    uint8_t trainerParty2Buf[6 * TEAMUP_POKE_FULL_DATA_SIZE];
    int32_t trainerParty2BufSize;
    int32_t trainerParty2Count;
    bool trainerParty2Valid;

    // Partner's trainer party data (received from Player B in TEAMUP_BATTLE_ACK)
    // Used by Player A for dual-trainer mode when trainers mismatch and have < 3 Pokemon.
    uint8_t partnerTrainerBuf[6 * TEAMUP_POKE_FULL_DATA_SIZE];
    int32_t partnerTrainerBufSize;
    int32_t partnerTrainerCount;
    bool partnerTrainerValid;
    int32_t partnerTrainerID;   // Player B's trainer ID (for normalTrainer AI/class data)

    // Active team-up battle state
    bool battlePending;             // waiting for partner's ACK during party exchange
    uint8_t battleType;             // 0 = wild, 1 = trainer
    uint8_t battleMultiMode;        // agreed BTL_MULTIMODE for this battle (2=PP_AA two
                                    // enemy clients; 6=PA_A2 single enemy trainer covering
                                    // both enemy positions). Decided by the initiator and
                                    // sent in the HEADER so both consoles never disagree.
    int32_t battleTrainerID;        // for trainer battles (enemyID0)
    int32_t battleTrainerID2;       // second trainer for already-double battles (enemyID1), 0 if single
    int32_t battleEffectID;         // BattleSetupEffectData index for intro/BGM (-1 = default)
    int32_t battleArenaID;          // arena background
    int32_t battleWeatherType;      // weather for battle

    // Sync-wait state machine
    SyncPhase syncPhase;            // current sync phase
    int32_t syncTrainerID;          // trainer ID we're waiting to sync at
    int32_t syncTrainerID2;         // second trainer ID (0 for single trainer)
    int32_t syncArenaID;            // arena ID from our SetupBattleTrainer
    int32_t syncWeatherType;        // weather from our SetupBattleTrainer
    int32_t syncEffectID;           // effect ID from our SetupBattleTrainer
    int32_t syncRandomTeamMode;     // our randomTeamMode at sync entry time
    int32_t syncZoneID;             // our zone ID at sync entry time
    bool partnerSyncReceived;       // true when partner's SYNC_WAIT arrived and matched
    bool partnerSyncMismatch;       // partner SYNC_WAIT arrived with different trainer ID
    int32_t partnerRandomTeamMode;  // partner's randomTeamMode (-1 = not received)
    int32_t partnerZoneID;          // partner's zone ID (-1 = not received)
    bool bypassTrainerFlag;         // true = bypass _IF_TR_FLAGON_JUMP/CALL for defeated trainers

    // Post-battle state
    bool isInitiator;               // true = lower station ID (initiates 0xC9 exchange)
    int32_t battleResult;           // cached BSP.result: 0=loss, 1=win, -1=not set
    int32_t battleGetMoney;         // cached BSP.getMoney

    void Clear() {
        memset(this, 0, sizeof(*this));
        partnerStation = -1;
        battleResult = -1;
        partnerTrainerValid = false;
        partnerTrainerID = 0;
        syncPhase = SyncPhase::SYNC_NONE;
        battleEffectID = -1;
        battleArenaID = -1;
        syncEffectID = -1;
        syncRandomTeamMode = -1;
        syncZoneID = -1;
        partnerRandomTeamMode = -1;
        partnerZoneID = -1;
    }
};

// Packet DataIDs 0xC8-0xCC are defined in overworld_multiplayer.h

// Sub-packet types for TEAMUP_BATTLE and TEAMUP_BATTLE_ACK (chunked, same as 0xC6)
static constexpr uint8_t TEAMUP_SUB_HEADER       = 0;  // Header: battleType, memberCount, MYSTATUS, encounter data
static constexpr uint8_t TEAMUP_SUB_POKE         = 1;  // Single Pokemon: pokeIndex + 344 bytes
static constexpr uint8_t TEAMUP_SUB_TRAINER_POKE = 2;  // Trainer Pokemon: pokeIndex + 344 bytes (initiator sends their trainer's party)
static constexpr uint8_t TEAMUP_SUB_TRAINER_POKE2 = 3; // Second real trainer's party (slot 3), genuine two-trainer double only

// BTL_MULTIMODE values used by team-up battles (indices into the game's baked
// position-cover tables — see PokemonPosition::GetPosCoverClientId_Double @0x20CEF00).
//   PP_AA (2): cover [0,1,2,3,5] — 2 human + 2 distinct AI enemy clients (slots 1,3).
//   PA_A2 (6): cover [0,1,2,1,5] — client 1 covers BOTH enemy positions (one trainer,
//              two mons from one origin, "Cyrus-style"); no client 3. Used for a
//              genuine single-trainer team-up encounter.
static constexpr uint8_t TEAMUP_MULTIMODE_PP_AA  = 2;
static constexpr uint8_t TEAMUP_MULTIMODE_SINGLE = 6;  // PA_A2

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

// Get the global team-up state
TeamUpState& overworldMPGetTeamUpState();

// Check if currently teamed up
bool overworldMPIsTeamedUp();

// Team-up handshake (called from interaction menu)
void overworldMPTeamUp(int32_t partnerStation);

// Disband team-up (send notification + clear state)
void overworldMPTeamUpDisband();

// Auto-disband check (called each tick from overworldMPUpdate)
void overworldMPTeamUpAutoDisband();

// Send team-up battle ACK (partner responding with their party + MYSTATUS)
void overworldMPSendTeamUpBattleAck(int32_t targetStation);

// Called from packet handler when TEAMUP_BATTLE arrives
void overworldMPOnTeamUpBattleReceived(int32_t fromStation, uint8_t* data, int32_t size);

// Called from packet handler when TEAMUP_BATTLE_ACK arrives
void overworldMPOnTeamUpBattleAckReceived(int32_t fromStation, uint8_t* data, int32_t size);

// Called from packet handler when TEAMUP_DISBAND arrives
void overworldMPOnTeamUpDisbandReceived(int32_t fromStation);

// Modify BSP for team-up double battle (PP_AA mode)
void overworldMPModifyBSPForTeamUp(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp,
                                    TeamUpState& tu, bool isInitiator);

// Clear the team-up battle state (called when battle scene ends)
void overworldMPClearTeamUpBattleState();

// Handle post-battle effects for team-up (party copy-back)
void overworldMPHandleTeamUpPostBattle();

// Save the full player party before team-up battle (for restoring non-participating Pokemon)
void overworldMPSaveFullParty();

// Tick function for deferred EncountStart — call each frame from main MP tick.
// Handles sync-wait phases, A-button cancel, and party exchange.
void overworldMPTickDeferredEncount();

// --- Sync-wait system ---

// Enter sync-wait state: pause event script, send SYNC_WAIT, show waiting message.
// Called from SetupBattleTrainer hook when a team-up trainer encounter is triggered.
void overworldMPEnterSyncWait();

// Cancel sync and proceed with solo battle (original BSP, no PP_AA).
// Called when player presses A during SYNC_WAITING, or on disconnect.
void overworldMPCancelSyncAndGoSolo();

// Called from packet handler when TEAMUP_SYNC_WAIT (0xCB) arrives
void overworldMPOnSyncWaitReceived(int32_t fromStation, int32_t trainerID,
                                    int32_t trainerID2, int32_t arenaID,
                                    int32_t weatherType, int32_t effectID,
                                    int32_t randomTeamMode, int32_t zoneID);

// Called from packet handler when TEAMUP_SYNC_CANCEL (0xCC) arrives
void overworldMPOnSyncCancelReceived(int32_t fromStation, uint8_t reason);

// --- Battle helpers (defined in trainer_double_battle.cpp) ---

// Fill a BSP party slot with trainer AI data
// normalTrainer @ 0x1F6EE00
inline void normalTrainer(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp, int32_t clientId, int32_t trainerID) {
    _ILExternal::external<void>(0x1F6EE00, bsp, clientId, trainerID);
}

// Split a single trainer's party between two AI slots for PP_AA double battles
void splitTrainerParty(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp, int slot1, int slot3);

// Apply the human-trainer color overrides for a PP_AA comm battle (TRAINER_DATA
// colorID + MyStatus byte + slot color arrays). Shared with mp_tower.cpp —
// the tower battle uses the identical human slot layout (0/2).
struct FieldPlayerNetData;
void owmpApplyTeamUpBattleColors(void* trData,
                                 Dpr::Battle::Logic::MyStatus::Array* statusArr,
                                 FieldPlayerNetData& remote, int32_t localColor,
                                 int32_t localSlot, int32_t partnerSlot);

// Arm/disarm the CreateLocalClient force-create hook (PP_AA + wireless AI
// slots). team_up arms it in its own BSP handlers; mp_tower arms it for tower
// rounds. Disarmed by overworldMPClearTeamUpBattleState after every battle.
void owmpTeamUpSetCommClientHookActive(bool active);

// True if the local party has a battle-eligible lead in the first TEAMUP_PARTY_LIMIT
// slots (non-egg, HP>0). Used to skip team-up when the player has no usable lead.
bool overworldMPLocalHasTeamUpLead();

// Install team-up hooks
void exl_team_up_main();

// Install trainer-flag-bypass hooks (lets a teamed-up partner re-fight
// already-defeated trainers during sync). Part of the overworld MP feature.
void exl_trainer_flag_bypass_main();
