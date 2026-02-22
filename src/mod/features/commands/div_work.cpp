#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FlagWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool DivWork(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_DIV_WK\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t initialValue = GetWorkOrIntValue(args->m_Items[1]);
    int32_t value = GetWorkOrIntValue(args->m_Items[2]);

    if (value != 0)
        FlagWork::SetWork(args->m_Items[1].fields.data, initialValue / value);

    return true;
}