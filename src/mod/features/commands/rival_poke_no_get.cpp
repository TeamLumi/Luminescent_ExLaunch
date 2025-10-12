#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool RivalPokeNoGet(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_RIVAL_POKE_NO_GET\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t starterId = PlayerWork::get_rivalPokeType();

    if (args->max_length >= 2)
    {
        SetWorkToValue(args->m_Items[1], (int32_t)starterId);
    }

    return true;
}