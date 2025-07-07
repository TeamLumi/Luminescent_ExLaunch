#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FlagWork.h"
#include "externals/TrainerWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"

bool TrainerDefeatedCount(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_TRAINER_DEFEATED_COUNT\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    auto gameMode = (int32_t)getCustomSaveData()->settings.gameMode;
    uint64_t rangeStart = 2000 * gameMode;
    uint64_t rangeEnd = rangeStart + 2000;

    int32_t count = 0;
    for (uint64_t i=rangeStart; i<rangeEnd; i++)
    {
        if (TrainerWork::GetWinFlag(i))
            count++;
    }

    SetWorkToValue(args->m_Items[1], count);

    return true;
}