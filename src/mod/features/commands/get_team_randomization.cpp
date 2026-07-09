#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"

bool GetTeamRandomization(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_GET_TEAM_RANDOMIZATION\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2) {
        auto randomTeamMode = (int32_t)getCustomSaveData()->settings.randomTeamMode;
        SetWorkToValue(args->m_Items[1], randomTeamMode);
    }

    return true;
}
