#include "exlaunch.hpp"
#include "logger/logger.h"
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"

#include "features/trainer_battle.h"

using namespace Dpr::Battle::Logic;
using Dpr::EvScript::EvDataManager;

struct PendingOverride {
    bool active;
    int32_t rule;
    int32_t multiMode;
};
static PendingOverride s_pending = { false, BTL_RULE_DERIVE, BTL_MULTIMODE_KEEP };

HOOK_DEFINE_TRAMPOLINE(SetupBattleTrainer) {
    static void Callback(BATTLE_SETUP_PARAM::Object *battleSetupParam, int32_t arenaID,
                         int32_t mapAttrib, int32_t weatherType, int32_t rule, int32_t enemyID0,
                         int32_t enemyID1, int32_t partnerID, MethodInfo *method)
    {
        PendingOverride pending = s_pending;
        s_pending.active = false;

        if (pending.active && pending.rule != BTL_RULE_DERIVE)
            rule = pending.rule;

        Orig(battleSetupParam, arenaID, mapAttrib, weatherType, rule, enemyID0, enemyID1, partnerID, method);

        // because multimode is assigned inside orig's setup it has to be overriden after
        if (pending.active && pending.multiMode != BTL_MULTIMODE_KEEP && battleSetupParam != nullptr)
            battleSetupParam->fields.multiMode = (uint8_t) pending.multiMode;
    }
};

void StartTrainerBattle(EvDataManager::Object* manager,
                        int32_t enemyID0, int32_t enemyID1, int32_t partnerID,
                        int32_t rule, int32_t multiMode)
{
    s_pending.active = (rule != BTL_RULE_DERIVE) || (multiMode != BTL_MULTIMODE_KEEP);
    s_pending.rule = rule;
    s_pending.multiMode = multiMode;

    manager->SetBattleReturn();
    manager->BattleTrainer(enemyID0, enemyID1, partnerID);
}

void exl_trainer_double_battles_main() {
    SetupBattleTrainer::InstallAtOffset(0x02c3b800);
}
