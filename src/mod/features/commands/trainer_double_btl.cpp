#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"

#include "features/commands/utils/cmd_utils.h"
#include "features/trainer_battle.h"
#include "logger/logger.h"

using namespace Dpr::Battle::Logic;

// _TRAINER_DOUBLE_BTL(opponent [, opponent2])
//   A single, self-contained command that starts a double battle via BattleTrainer,
//   replacing the old "set FLAG_TRAINER_DOUBLE then _TRAINER_BTL_SET" two-step idiom.
bool TrainerDoubleBattle(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_TRAINER_DOUBLE_BTL\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    // args->m_Items[0] is the command ID; real arguments start at index 1.
    if (args->max_length >= 2)
    {
        int32_t opponentID = GetWorkOrIntValue(args->m_Items[1]);
        int32_t opponent2ID = (args->max_length >= 3) ? GetWorkOrIntValue(args->m_Items[2]) : 0;

        Logger::log("  Opponent ID: %d\n", opponentID);
        Logger::log("  Opponent2 ID: %d\n", opponent2ID);

        // One trainer  -> force DOUBLE so a single trainer fights with two Pokemon.
        // Two trainers -> let BattleTrainer derive DOUBLE from the 2nd enemy ID itself.
        int32_t rule = (opponent2ID != 0) ? BTL_RULE_DERIVE : (int32_t) BtlRule::BTL_RULE_DOUBLE;

        StartTrainerBattle(manager, opponentID, opponent2ID, /*partnerID*/ 0, rule, BTL_MULTIMODE_KEEP);
    }
    else
    {
        Logger::log("  _TRAINER_DOUBLE_BTL: expected at least 1 arg (opponent)\n");
    }

    return true;
}
