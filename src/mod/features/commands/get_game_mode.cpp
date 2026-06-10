#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"

bool GetGameMode(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_GET_GAME_MODE\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2) {
        auto gameMode = (int32_t)getCustomSaveData()->settings.gameMode;
        SetWorkToValue(args->m_Items[1], gameMode);
    }

    return true;
}
