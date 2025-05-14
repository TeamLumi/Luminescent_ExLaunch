#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "romdata/romdata.h"

bool PlayerStarterMonsNoFormNo(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_FIRST_MONSNO_FORMNO\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t starterId = PlayerWork::get_defaultPokeType();
    RomData::Starter starter = GetStarter(starterId);

    if (args->max_length >= 2)
    {
        SetWorkToValue(args->m_Items[1], (int32_t)starter.monsNo);
    }

    if (args->max_length >= 3)
    {
        SetWorkToValue(args->m_Items[2], (int32_t)starter.formNo);
    }

    return true;
}