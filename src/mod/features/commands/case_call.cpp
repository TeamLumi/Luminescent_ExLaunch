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
        auto argType = static_cast<EvData::ArgType>(args->m_Items[1].fields.argType);

        if (argType == EvData::ArgType::Work) {
            workNoValue = args->m_Items[1].fields.data;
        }
        else if (argType == EvData::ArgType::Float) {
            workNoValue = static_cast<float>(args->m_Items[1].fields.data);
        }
        else {
            workNoValue = 0;
        }

        EvData::Aregment::Object arg3 = args->m_Items[2];
        System::String::Object* label = System::String::Create("");

        if (static_cast<EvData::ArgType>(arg3.fields.argType) == EvData::ArgType::String) {
            EvData::Object* evData = manager->fields._evData->fields._EvData;
            label = evData->GetString(arg3.fields.data);
        }

        if (switchWorkIndex == workNoValue) {
            manager->CallLabel(label);
        }

    }
    return true;
}