#include "externals/Dpr/EvScript/EvDataManager.h"
#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "commands.h"
#include "save/save.h"

#include "exlaunch.hpp"

bool FTR_HALL_INC_ROUND(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_INC_ROUND] \n");
    auto save = &getCustomSaveData()->battleHall;

    save->currentRound += 1;

    Logger::log("[_FTR_HALL_INC_ROUND] Current round: %d\n", (save->currentRound) + 1);

    return true;
}

