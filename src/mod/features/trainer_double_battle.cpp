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

// BTL_MULTIMODE_PA_A2: Player + AI Partner vs single AI enemy (enemy controls 2 Pokemon)
constexpr int32_t BTL_MULTIMODE_PA_A2 = 6;

static bool s_multiSingleMode = false;

HOOK_DEFINE_TRAMPOLINE(SetupBattleTrainer) {
    static void Callback(BATTLE_SETUP_PARAM::Object *battleSetupParam, int32_t arenaID,
                         int32_t mapAttrib, int32_t weatherType, int32_t rule, int32_t enemyID0, int32_t enemyID1,
                         int32_t partnerID, MethodInfo *method)
    {
        // FLAG_TRAINER_DOUBLE: Force any trainer battle to use double battle rules
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_TRAINER_DOUBLE))
        {
            rule = (int32_t) BtlRule::BTL_RULE_DOUBLE;
        }

        // FLAG_MULTI_BATTLE_SINGLE: Multi battle with partner vs single opponent
        // Partner and player each use 1 Pokemon, enemy uses 2 Pokemon
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_MULTI_BATTLE_SINGLE))
        {
            rule = (int32_t) BtlRule::BTL_RULE_DOUBLE;
            partnerID = GetMultiBattlePartnerID();
            s_multiSingleMode = true;

            Logger::log("[MultiSingle] Partner: %d, Enemy: %d\n", partnerID, enemyID0);

            FlagWork::SetFlag(FlagWork_Flag::FLAG_MULTI_BATTLE_SINGLE, false);
            ClearMultiBattlePartnerID();
        }

        Orig(battleSetupParam, arenaID, mapAttrib, weatherType, rule, enemyID0, enemyID1, partnerID, method);

        // Set multiMode after setup to enable PA_A2 (partner + single enemy with 2 Pokemon)
        if (s_multiSingleMode && battleSetupParam != nullptr)
        {
            battleSetupParam->fields.multiMode = BTL_MULTIMODE_PA_A2;
            s_multiSingleMode = false;
        }
    }
};

void exl_trainer_double_battles_main() {
    SetupBattleTrainer::InstallAtOffset(0x02c3b800);
}
