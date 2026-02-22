#include "exlaunch.hpp"

#include "externals/Dpr/BattleMatching/BattleMatchingWork.h"
#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/commands/commands.h"
#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"

bool FTR_HALL_GET_STREAK_MON(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_GET_STREAK_MON] \n");
    auto save = &getCustomSaveData()->battleHall;

    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 2) {
        int32_t savedMonsNo = save->streakPokePID;
        Dpr::BattleMatching::BattleMatchingWork::getClass()->initIfNeeded();
        auto pokeParam = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams;
        auto orderIndexList = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->orderIndexList;
        auto playerPoke = pokeParam->m_Items[orderIndexList->m_Items[0]];
        int32_t compareMonsNo = playerPoke->fields.m_accessor->GetMonsNo();

        Logger::log("[_FTR_HALL_GET_STREAK_MON] savedPokeID: %d vs. comparePID: %d.\n", savedMonsNo, compareMonsNo);

        if (savedMonsNo == compareMonsNo || savedMonsNo == -1) {
            SetWorkToValue(args->m_Items[1], 1);
        }

        else {
            SetWorkToValue(args->m_Items[1], 0);
        }
    }

    return true;
}

