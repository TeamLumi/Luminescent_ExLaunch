#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "commands.h"
#include "save/save.h"
#include "externals/Dpr/BattleMatching/BattleMatchingWork.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"

#include "exlaunch.hpp"

bool FTR_HALL_STREAK_MON_NAME(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_STREAK_MON_NAME] \n");
    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 1) {
        auto tagIndex = args->m_Items[1].fields.data;
        auto save = &getCustomSaveData()->battleHall;
        auto streakMonsNo = save->streakPokePID;
        Dpr::Message::MessageWordSetHelper::SetMonsNameWord(tagIndex, streakMonsNo);
    }
    return true;
}

