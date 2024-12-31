#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "commands.h"
#include "save/save.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"

#include "exlaunch.hpp"

bool FTR_HALL_POOL_REMOVE(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_POOL_REMOVE] \n");
    auto save = &getCustomSaveData()->battleHall;
    auto currentType = FlagWork::GetWork(FlagWork_Work::WK_BATTLE_HALL_CURRENT_TYPE);
    auto currentMon = FlagWork::GetWork(FlagWork_Work::WK_BATTLE_HALL_SELECTED_MON);
    auto currentRank = save->currentRank[currentType];

    save->poolManager.addBlackListedMonsNo(currentType, currentRank, currentMon);

    return true;
}

