#include "exlaunch.hpp"
#include "logger/logger.h"
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/PlayerWork.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"

#include "features/team_up.h"
#include "features/overworld_multiplayer.h"

using namespace Dpr::Battle::Logic;

// normalTrainer is now declared inline in team_up.h

// Split trainer party between two AI slots (for single-trainer team-up)
// Declaration in team_up.h — called from team_up.cpp as well
void splitTrainerParty(BATTLE_SETUP_PARAM::Object* bsp, int slot1, int slot3) {
    auto* fields = &bsp->instance()->fields;
    if (fields->party == nullptr || (uint32_t)slot3 >= fields->party->max_length) return;

    auto* party1 = fields->party->m_Items[slot1];
    if (party1 == nullptr) return;

    auto* party3 = fields->party->m_Items[slot3];
    if (party3 == nullptr) {
        // Create a new PokeParty for slot 3 (no normalTrainer was called for this slot)
        party3 = Pml::PokeParty::newInstance();
        fields->party->m_Items[slot3] = party3;
        Logger::log("[TeamUp] Created new party for slot %d\n", slot3);
    }

    int32_t total = party1->fields.m_memberCount;

    if (total <= 1) {
        // Only 1 Pokemon: keep it in slot 1, copy to slot 3 using PokemonParam copy ctor
        party1->fields.m_memberCount = 1;
        auto* src = party1->GetMemberPointer(0);
        auto* dst = party3->GetMemberPointer(0);
        if (src != nullptr && dst != nullptr) {
            dst->ctor(src);
        }
        party3->fields.m_memberCount = 1;
        Logger::log("[TeamUp] Trainer has 1 Pokemon — duplicated to both AI slots\n");
        return;
    }

    // Split: slot 1 gets first ceil(N/2), slot 3 gets remaining
    int32_t half = (total + 1) / 2;

    // Copy Pokemon from party1[half..N] into party3[0..] using PokemonParam copy ctor
    for (int i = 0; i < total - half; i++) {
        auto* src = party1->GetMemberPointer(half + i);
        auto* dst = party3->GetMemberPointer(i);
        if (src != nullptr && dst != nullptr) {
            dst->ctor(src);
        }
    }
    party1->fields.m_memberCount = half;
    party3->fields.m_memberCount = total - half;

    Logger::log("[TeamUp] Split trainer party: slot %d=%d, slot %d=%d (total %d)\n",
                slot1, half, slot3, total - half, total);
}

HOOK_DEFINE_TRAMPOLINE(SetupBattleTrainer) {
    static void Callback(BATTLE_SETUP_PARAM::Object *battleSetupParam, int32_t arenaID,
                         int32_t mapAttrib, int32_t weatherType, int32_t rule, int32_t enemyID0, int32_t enemyID1,
                         int32_t partnerID, MethodInfo *method)
    {
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_TRAINER_DOUBLE))
        {
            rule = (int32_t) BtlRule::BTL_RULE_DOUBLE;
        }

        bool doTeamUp = false;
        if (overworldMPIsTeamedUp() && partnerID == 0 &&
            !PlayerWork::GetSystemFlag((int32_t)FlagWork_SysFlag::SYS_FLAG_PAIR)) {
            auto& tu = overworldMPGetTeamUpState();
            doTeamUp = true;

            // Guard: if a team-up sync/battle is already in flight, don't double-trigger.
            if (tu.battlePending || tu.syncPhase != SyncPhase::SYNC_NONE) {
                doTeamUp = false;
                Logger::log("[TeamUp] Skipping: sync/battle already in flight (phase=%d, pending=%d)\n",
                            (int)tu.syncPhase, (int)tu.battlePending);
            }
        }

        // No area check — always enter sync-wait. If the partner is elsewhere,
        // the player waits until partner arrives or presses B to go solo.

        if (doTeamUp) {
            // Do NOT force DOUBLE yet — keep original rule so solo fallback works.
            // DOUBLE + PP_AA modifications happen in SYNC_MATCHED phase.
            Logger::log("[TeamUp] SetupBattleTrainer: team-up sync-wait (enemy=%d, rule=%d)\n",
                        enemyID0, rule);

            // Save full party BEFORE Orig — non-participating Pokemon (slots 3+) will
            // be destroyed when the battle engine writes back only TEAMUP_PARTY_LIMIT members.
            overworldMPSaveFullParty();

            // Set pre-Orig guard BEFORE calling Orig — if SetupBattleTrainer internally
            // calls FieldManager::EncountStart, DeferEncountStart needs to catch it
            // even though syncPhase hasn't been set yet (it's set in overworldMPEnterSyncWait
            // which runs AFTER Orig).
            extern bool s_teamUpEncountPending;
            s_teamUpEncountPending = true;
        }

        Orig(battleSetupParam, arenaID, mapAttrib, weatherType, rule, enemyID0, enemyID1, partnerID, method);

        if (!doTeamUp) return;

        auto& tu = overworldMPGetTeamUpState();

        // Cache BSP pointer — it will be modified when sync match is confirmed
        extern Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* s_teamUpBSP;
        s_teamUpBSP = battleSetupParam;

        // Store trainer battle info (isInitiator determined by tie-break in SYNC_MATCHED)
        tu.battleType = 1; // trainer
        tu.battleTrainerID = enemyID0;
        tu.battleTrainerID2 = enemyID1; // 0 for single trainer
        tu.battleArenaID = arenaID;
        tu.battleWeatherType = weatherType;

        // Capture BattleEffectID from tr_data for correct intro/BGM
        tu.battleEffectID = -1;
        auto* fields = &battleSetupParam->instance()->fields;
        if (fields->tr_data != nullptr) {
            uint8_t* trArr = (uint8_t*)fields->tr_data;
            uint32_t trLen = *(uint32_t*)(trArr + 0x18);
            Logger::log("[TeamUp] tr_data len=%d\n", trLen);
            for (uint32_t i = 0; i < trLen; i++) {
                void* td = *(void**)(trArr + 0x20 + i * 8);
                if (td != nullptr) {
                    int32_t eff = _ILExternal::external<int32_t>(0x1AC45B0, td);
                    Logger::log("[TeamUp] tr_data[%d] effect=%d\n", i, eff);
                    if (tu.battleEffectID < 0) {
                        tu.battleEffectID = eff;
                    }
                }
            }
            if (tu.battleEffectID >= 0) {
                Logger::log("[TeamUp] Captured BattleEffectID=%d from trainer %d\n",
                            tu.battleEffectID, enemyID0);
            }
        }

        // Enter sync-wait: send SYNC_WAIT packet, show "Waiting for partner..." message.
        // DeferEncountStart will pause the event script until sync resolves.
        overworldMPEnterSyncWait();

        Logger::log("[TeamUp] SetupBattleTrainer: enemy0=%d enemy1=%d effectID=%d, entering sync-wait\n",
                    enemyID0, enemyID1, tu.battleEffectID);
    }
};

void exl_trainer_double_battles_main() {
    SetupBattleTrainer::InstallAtOffset(0x02c3b800);
}
