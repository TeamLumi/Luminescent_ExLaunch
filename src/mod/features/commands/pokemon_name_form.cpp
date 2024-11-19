#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"

bool PokemonNameForm(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_POKEMON_NAME_FORM\n");
    system_load_typeinfo(0x44ac);

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 4) {
        auto tagIndex = GetWorkOrIntValue(args->m_Items[1]);
        auto monsNo = GetWorkOrIntValue(args->m_Items[2]);
        auto formNo = GetWorkOrIntValue(args->m_Items[3]);

        auto sheetPersonal = Pml::Personal::PersonalSystem::GetPersonalData(monsNo, formNo);
        auto formIndex = sheetPersonal->fields.form_index;

        int32_t formTextIndex = (formNo == 0 || formIndex == 0) ? monsNo : (formNo + formIndex - 1);

        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
        auto msgManager = Dpr::Message::MessageWordSetHelper::getClass()->static_fields->manager;
        auto msgFile = msgManager->GetMsgFile(System::String::Create("ss_zkn_form"));

        Dpr::Message::MessageWordSetHelper::SetStringWordFromMsgFile(tagIndex, msgFile, formTextIndex);
    }
    return true;
}