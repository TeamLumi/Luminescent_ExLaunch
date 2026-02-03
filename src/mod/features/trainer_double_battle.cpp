#include "exlaunch.hpp"
#include "Logger/logger.h"
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/PlayerWork.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"

// External functions from trainer_multi_btl_single.cpp
extern int32_t GetMultiBattlePartnerID();
extern void ClearMultiBattlePartnerID();

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_TRAMPOLINE(SetupBattleTrainer) {
    static void Callback(BATTLE_SETUP_PARAM::Object *battleSetupParam, int32_t arenaID,
                         int32_t mapAttrib, int32_t weatherType, int32_t rule, int32_t enemyID0, int32_t enemyID1,
                         int32_t partnerID, MethodInfo *method)
    {
        // Handle FLAG_TRAINER_DOUBLE - forces double battle rule
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_TRAINER_DOUBLE))
        {
            rule = (int32_t) BtlRule::BTL_RULE_DOUBLE;
        }

        // Handle FLAG_MULTI_BATTLE_SINGLE - multi battle with partner vs single opponent (2v2)
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_MULTI_BATTLE_SINGLE))
        {
            Logger::log("SetupBattleTrainer: Multi Battle Single Opponent Mode\n");

            // Set battle rule to double (2v2)
            rule = (int32_t) BtlRule::BTL_RULE_DOUBLE;

            // Inject the stored partner ID
            partnerID = GetMultiBattlePartnerID();

            // Ensure only one opponent (enemyID1 should be 0)
            enemyID1 = 0;

            Logger::log("  Partner ID: %d\n", partnerID);
            Logger::log("  Enemy ID: %d\n", enemyID0);

            // Clear the flag after use
            FlagWork::SetFlag(FlagWork_Flag::FLAG_MULTI_BATTLE_SINGLE, false);

            // Clear the stored partner ID
            ClearMultiBattlePartnerID();
        }

        Orig(battleSetupParam, arenaID, mapAttrib, weatherType, rule, enemyID0, enemyID1, partnerID, method);
    }

};

void exl_trainer_double_battles_main() {
    SetupBattleTrainer::InstallAtOffset(0x02c3b800);
}
