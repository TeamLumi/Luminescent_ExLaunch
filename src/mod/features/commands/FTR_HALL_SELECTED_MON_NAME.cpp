#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "commands.h"
#include "save/save.h"
#include "externals/Dpr/BattleMatching/BattleMatchingWork.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"

#include "exlaunch.hpp"

bool FTR_HALL_SELECTED_MON_NAME(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_SELECTED_MON_NAME] \n");
    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 1) {
        auto tagIndex = args->m_Items[1].fields.data;
        Dpr::BattleMatching::BattleMatchingWork::getClass()->initIfNeeded();
        auto pokeParam = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams;
        auto orderIndexList = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->orderIndexList;
        auto playerPoke = pokeParam->m_Items[orderIndexList->m_Items[0]];
        Dpr::Message::MessageWordSetHelper::SetMonsNameWord(tagIndex, playerPoke->fields.m_accessor->GetMonsNo());
    }
    return true;
}

