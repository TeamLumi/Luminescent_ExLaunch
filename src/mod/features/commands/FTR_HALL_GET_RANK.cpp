#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "commands.h"
#include "save/save.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"

#include "exlaunch.hpp"

bool FTR_HALL_RANK_NUM_NAME(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_RANK_NUM_NAME] \n");
    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 2) {
        auto save = &getCustomSaveData()->battleHall;
        int32_t selectedType = args->m_Items[1].fields.data;
        int32_t tagIndex = args->m_Items[2].fields.data;
        Rank rank = save->currentRank[selectedType];
        Logger::log("[_FTR_HALL_RANK_NUM_NAME] tagIndex: %d, Rank value: %d.\n", tagIndex, rank+1);

        Dpr::Message::MessageWordSetHelper::SetDigitWord(tagIndex, rank+1);

    }
    return true;
}

