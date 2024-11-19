#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/ZukanWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"

bool GetCostumeGender(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_GET_COSTUME_GENDER\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);
        if (args->max_length >= 3)
            SetWorkToValue(args->m_Items[2], index % 2);
    }

    return true;
}