#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/FlagWork_Enums.h"

#include "features/commands/commands.h"
#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"

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

