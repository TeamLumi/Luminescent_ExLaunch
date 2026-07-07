#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"

#include "features/commands/utils/cmd_utils.h"
#include "features/trainer_battle.h"
#include "logger/logger.h"

using namespace Dpr::Battle::Logic;

// _TRAINER_MULTI_BTL_SINGLE(partner, opponent)
//   Player + AI partner (1 Pokemon each) vs a single opponent trainer that sends out
//   2 Pokemon from one origin (PA_A2). Unlike the old implementation this command both
//   sets up AND starts the battle in one step by calling StartTrainerBattle directly --
//   no FlagWork state and no follow-up _TRAINER_BTL_SET required.
bool TrainerMultiBattleSingle(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_TRAINER_MULTI_BTL_SINGLE\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    // args->m_Items[0] is the command ID; real arguments start at index 1.
    if (args->max_length >= 3)
    {
        int32_t partnerID = GetWorkOrIntValue(args->m_Items[1]);
        int32_t opponentID = GetWorkOrIntValue(args->m_Items[2]);

        Logger::log("  Partner ID: %d\n", partnerID);
        Logger::log("  Opponent ID: %d\n", opponentID);

        // One opponent trainer that sends out 2 Pokemon, so it goes in enemyID0 and
        // enemyID1 stays 0 -- a non-zero enemyID1 would make BattleTrainer treat it as two
        // separate enemy trainers. BattleTrainer derives SINGLE for one enemy (and would
        // then ignore the partner), so force DOUBLE, and set PA_A2 for the "1 trainer / 2
        // mons / 1 AI ally" slot layout (vanilla mode-6 leaves the ally AI-controlled).
        StartTrainerBattle(manager, /*enemyID0*/ opponentID, /*enemyID1*/ 0, partnerID,
                           (int32_t) BtlRule::BTL_RULE_DOUBLE, BTL_MULTIMODE_PA_A2);
    }
    else
    {
        Logger::log("  _TRAINER_MULTI_BTL_SINGLE: expected 2 args (partner, opponent)\n");
    }

    return true;
}
