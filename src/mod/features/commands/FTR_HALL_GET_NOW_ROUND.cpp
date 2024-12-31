#include "externals/Dpr/EvScript/EvDataManager.h"
#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "commands.h"
#include "save/save.h"

#include "exlaunch.hpp"

bool FTR_HALL_GET_NOW_ROUND(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_GET_NOW_ROUND] \n");
    auto save = &getCustomSaveData()->battleHall;


    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 2) {
        HallRound currentNowRound = save->getNowRound();
        SetWorkToValue(args->m_Items[1], currentNowRound+1);
    }

    return true;
}

