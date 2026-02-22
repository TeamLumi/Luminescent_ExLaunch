#include "exlaunch.hpp"

#include "externals/Dpr/BattleMatching/BattleMatchingWork.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"

#include "features/commands/commands.h"
#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"

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

