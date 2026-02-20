#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/Pml/PokeParty.h"

// ---------------------------------------------------------------------------
// Team-Up: Cooperative overworld multiplayer (double battles)
// ---------------------------------------------------------------------------
// When two players "Team Up", wild and trainer encounters become double
// battles with BTL_MULTIMODE_PP_AA (mode 2): both humans control one
// Pokemon each, the other two slots are AI (wild/trainer).

// PokePara full data size (same as overworld_multiplayer.cpp)
static constexpr int32_t TEAMUP_POKE_FULL_DATA_SIZE = 344;

// Max Pokemon per player in team-up battles (first 3 of party)
static constexpr int32_t TEAMUP_PARTY_LIMIT = 3;

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

    // Active team-up battle state
    bool battlePending;             // waiting for partner to enter battle
    uint8_t battleType;             // 0 = wild, 1 = trainer
    int32_t battleTrainerID;        // for trainer battles (enemyID0)
    int32_t battleTrainerID2;       // second trainer for already-double battles (enemyID1), 0 if single
    int32_t battleEffectID;         // BattleSetupEffectData index for intro/BGM (-1 = default)
    int32_t battleArenaID;          // arena background
    int32_t battleWeatherType;      // weather for battle

    // Post-battle state
    bool isInitiator;               // true = Player A (has event script), false = Player B
    int32_t battleResult;           // cached BSP.result: 0=loss, 1=win, -1=not set
    int32_t battleGetMoney;         // cached BSP.getMoney

    void Clear() {
        memset(this, 0, sizeof(*this));
        partnerStation = -1;
        battleResult = -1;
    }
};

// Packet DataIDs 0xC8-0xCA are defined in overworld_multiplayer.h

// Sub-packet types for TEAMUP_BATTLE and TEAMUP_BATTLE_ACK (chunked, same as 0xC6)
static constexpr uint8_t TEAMUP_SUB_HEADER   = 0;  // Header: battleType, memberCount, MYSTATUS, encounter data
static constexpr uint8_t TEAMUP_SUB_POKE     = 1;  // Single Pokemon: pokeIndex + 344 bytes

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

// Send team-up battle initiation to partner (trainer encounter)
void overworldMPSendTeamUpBattleTrainer(int32_t targetStation,
                                         int32_t enemyID0, int32_t arenaID,
                                         int32_t weatherType);

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

// Partner-side: build BSP and enter battle scene
void overworldMPSetupTeamUpBattle();

// Clear the team-up battle state (called when battle scene ends)
void overworldMPClearTeamUpBattleState();

// Handle post-battle effects for team-up (party copy-back, Player B rewards/whiteout)
void overworldMPHandleTeamUpPostBattle();

// Install team-up hooks (SetupBattleWild trampoline)
void exl_team_up_main();
