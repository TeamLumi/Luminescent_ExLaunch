#include "exlaunch.hpp"
#include "logger/logger.h"
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/PlayerWork.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"


using namespace Dpr::Battle::Logic;

HOOK_DEFINE_TRAMPOLINE(SetupBattleTrainer) {
    static void Callback(BATTLE_SETUP_PARAM::Object *battleSetupParam, int32_t arenaID,
                         int32_t mapAttrib, int32_t weatherType, int32_t rule, int32_t enemyID0, int32_t enemyID1,
                         int32_t partnerID, MethodInfo *method)
    {
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_TRAINER_DOUBLE))
        {
            rule = (int32_t) BtlRule::BTL_RULE_DOUBLE;
        }
        Orig(battleSetupParam, arenaID, mapAttrib, weatherType, rule, enemyID0, enemyID1, partnerID, method);
    }

};

void exl_trainer_double_battles_main() {
    SetupBattleTrainer::InstallAtOffset(0x02c3b800);
}
