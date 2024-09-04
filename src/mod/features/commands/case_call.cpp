#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FlagWork_Enums.h"
#include "externals/FlagWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"


bool CaseCall(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_CASE_CALL\n");
    system_load_typeinfo(0x45dc);
    EvData::Aregment::Array* args = manager->fields._evArg;
    int32_t switchWorkIndex = FlagWork::GetWork(manager->fields._switch_work_index);
    int32_t workNoValue;

    if (args->max_length >= 3)
    {

        workNoValue = GetWorkOrIntValue(args->m_Items[1]);
        System::String::Object* label = GetStringText(manager, args->m_Items[2]);

        if (switchWorkIndex == workNoValue) {
            manager->CallLabel(label);
        }

    }
    return true;
}