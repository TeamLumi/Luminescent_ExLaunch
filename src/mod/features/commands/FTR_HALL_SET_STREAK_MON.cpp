#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "commands.h"
#include "save/save.h"
#include "externals/Dpr/BattleMatching/BattleMatchingWork.h"

#include "exlaunch.hpp"

bool FTR_HALL_SET_STREAK_MON(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_SET_STREAK_MON] \n");
    auto save = &getCustomSaveData()->battleHall;
    Dpr::BattleMatching::BattleMatchingWork::getClass()->initIfNeeded();
    auto pokeParam = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams;
    auto orderIndexList = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->orderIndexList;
    auto playerPoke = pokeParam->m_Items[orderIndexList->m_Items[0]];
    save->streakPokePID = playerPoke->fields.m_accessor->GetMonsNo();
    return true;
}

