#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"

#include "features/commands/utils/utils.h"

#include "logger/logger.h"
#include "save/save.h"

bool AYouName(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_AYOU_NAME\n");
    system_load_typeinfo(0x44ac);

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2) {
        auto tagIndex = GetWorkOrIntValue(args->m_Items[1]);

        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
        Dpr::Message::MessageWordSetHelper::SetStringWord(tagIndex, System::String::Create(
                getCustomSaveData()->ayou.name));

    }

    return true;
}