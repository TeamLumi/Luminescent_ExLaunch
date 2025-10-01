#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool InstallCheck(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_INSTALL_CHECK\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        SetWorkToValue(args->m_Items[1], 69);
    }

    return true;
}