#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"

#include "features/commands/commands.h"
#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"

bool FTR_HALL_POOL_REMOVE(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_POOL_REMOVE] \n");
    auto save = &getCustomSaveData()->battleHall;
    auto currentType = FlagWork::GetWork(FlagWork_Work::WK_BATTLE_HALL_CURRENT_TYPE);
    auto currentMon = FlagWork::GetWork(FlagWork_Work::WK_BATTLE_HALL_SELECTED_MON);
    auto currentRank = save->currentRank[currentType];

    save->poolManager.addBlackListedMonsNo(currentType, currentRank, currentMon);

    return true;
}

