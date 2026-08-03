#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"

#include "features/commands/utils/cmd_utils.h"
#include "features/trainer_battle.h"
#include "logger/logger.h"

using namespace Dpr::Battle::Logic;

bool TrainerBtlSetExtra(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_TRAINER_BTL_SET_EXTRA\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length < 2)
    {
        Logger::log("  _TRAINER_BTL_SET_EXTRA: expected at least 1 arg (enemyA)\n");
        return true;
    }

    int32_t enemyA = GetWorkOrIntValue(args->m_Items[1]);
    int32_t ally   = (args->max_length >= 3) ? GetWorkOrIntValue(args->m_Items[2]) : 0;
    int32_t enemyB = (args->max_length >= 4) ? GetWorkOrIntValue(args->m_Items[3]) : 0;

    int32_t rule;
    if (args->max_length >= 5)
        rule = (GetWorkOrIntValue(args->m_Items[4]) == (int32_t) BtlRule::BTL_RULE_DOUBLE)
                   ? (int32_t) BtlRule::BTL_RULE_DOUBLE
                   : (int32_t) BtlRule::BTL_RULE_SINGLE;
    else
        rule = (ally != 0 || enemyB != 0) ? (int32_t) BtlRule::BTL_RULE_DOUBLE
                                          : (int32_t) BtlRule::BTL_RULE_SINGLE;

    // A given ally but only one enemy will make the enemy trainer send out 2 pokemon
    int32_t multiMode = BTL_MULTIMODE_KEEP;
    if (rule == (int32_t) BtlRule::BTL_RULE_DOUBLE && ally != 0 && enemyB == 0)
        multiMode = BTL_MULTIMODE_PA_A2;

    Logger::log("  enemyA: %d, ally: %d, enemyB: %d, rule: %d, multiMode: %d\n",
                enemyA, ally, enemyB, rule, multiMode);

    StartTrainerBattle(manager, enemyA, enemyB, ally, rule, multiMode);

    return true;
}
