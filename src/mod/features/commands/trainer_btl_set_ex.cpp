#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"

#include "features/commands/utils/cmd_utils.h"
#include "features/trainer_battle.h"
#include "logger/logger.h"

using namespace Dpr::Battle::Logic;

// _TRAINER_BTL_SET_EX(enemyA, ally, enemyB [, rule])
//   One unified trainer-battle command. The battle type is derived from which of the three
//   trainer-ID client slots are filled (client 0 is always the player):
//
//     enemyA  ally  enemyB | battle
//     ------  ----  ------ | ------------------------------------------------
//     set     0     0      | single (1v1)                    -- pass rule=DOUBLE for
//                          |                                    double vs a single trainer
//     set     0     set    | doubles: player vs enemyA + enemyB
//     set     set   set    | multi:   player + ally vs enemyA + enemyB
//     set     set   0      | PA_A2:   player + ally vs enemyA (enemyA sends 2 Pokemon)
//
//   `rule` is optional. When omitted the rule is derived (DOUBLE if any 2nd slot is filled,
//   else SINGLE); pass it explicitly to disambiguate the only ambiguous slot layout --
//   a single enemy that should nonetheless fight a DOUBLE battle. multiMode (PA_A2) is
//   selected automatically from the slots and never needs to be specified.
//
// Arguments:
//   [Work, Number] enemyA:        first enemy trainer (client 1). Required.
//   [Work, Number] ally:          partner trainer (client 2), or 0 for none.
//   [Work, Number] enemyB:        second enemy trainer (client 3), or 0 for none.
//   [Work, Number] rule (opt.):   BtlRule override (0 = SINGLE, 1 = DOUBLE).
bool TrainerBattleSetEx(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_TRAINER_BTL_SET_EX\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    // Index 0 is the command ID; enemyA (index 1) is the minimum required argument.
    if (args->max_length < 2)
    {
        Logger::log("  _TRAINER_BTL_SET_EX: expected at least 1 arg (enemyA)\n");
        return true;
    }

    int32_t enemyA = GetWorkOrIntValue(args->m_Items[1]);
    int32_t ally   = (args->max_length >= 3) ? GetWorkOrIntValue(args->m_Items[2]) : 0;
    int32_t enemyB = (args->max_length >= 4) ? GetWorkOrIntValue(args->m_Items[3]) : 0;

    // Derive the rule unless the script supplied an explicit override. An explicit rule is
    // clamped to the two values SetupBattleTrainer actually handles (SINGLE/DOUBLE); any
    // other value would fall through the game's setup branches and leave the battle unset.
    int32_t rule;
    if (args->max_length >= 5)
        rule = (GetWorkOrIntValue(args->m_Items[4]) == (int32_t) BtlRule::BTL_RULE_DOUBLE)
                   ? (int32_t) BtlRule::BTL_RULE_DOUBLE
                   : (int32_t) BtlRule::BTL_RULE_SINGLE;
    else
        rule = (ally != 0 || enemyB != 0) ? (int32_t) BtlRule::BTL_RULE_DOUBLE
                                          : (int32_t) BtlRule::BTL_RULE_SINGLE;

    // A partner facing a single enemy trainer is the PA_A2 layout (that enemy sends 2 mons);
    // every other topology is handled by the game's own setup path.
    int32_t multiMode = BTL_MULTIMODE_KEEP;
    if (rule == (int32_t) BtlRule::BTL_RULE_DOUBLE && ally != 0 && enemyB == 0)
        multiMode = BTL_MULTIMODE_PA_A2;

    Logger::log("  enemyA: %d, ally: %d, enemyB: %d, rule: %d, multiMode: %d\n",
                enemyA, ally, enemyB, rule, multiMode);

    // BattleTrainer takes (enemyID0, enemyID1, partnerID): ally is the partner, enemyB the 2nd enemy.
    StartTrainerBattle(manager, /*enemyID0*/ enemyA, /*enemyID1*/ enemyB, /*partnerID*/ ally,
                       rule, multiMode);

    return true;
}
