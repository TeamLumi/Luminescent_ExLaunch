#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/UI/BoxWindow.h"
#include "externals/FlagWork_Enums.h"

#include "features/commands/commands.h"
#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"

bool FTR_HALL_SET_RANK(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_SET_RANK] \n");
    auto save = &getCustomSaveData()->battleHall;
    auto currentType = FlagWork::GetWork(FlagWork_Work::WK_BATTLE_HALL_CURRENT_TYPE);
    auto currentRank = save->currentRank[currentType];

    if (currentRank < RANK_COUNT) {
        Rank newRank = static_cast<Rank>(static_cast<int>(currentRank) + 1);
        Logger::log("[_FTR_HALL_SET_RANK] Increasing %s rank to %d.\n", TYPES[currentType], newRank + 1);
        save->setRank(currentType, newRank);
    }
    return true;
}

