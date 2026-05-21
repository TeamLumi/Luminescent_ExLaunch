#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/UIBag.h"
#include "externals/Dpr/UI/UIManager.h"

void UseCustomKeyItem(int32_t itemId, bool fromBag, Dpr::UI::UIBag::__c__DisplayClass127_1::Object* bagDisplayClass) {

    system_load_typeinfo(0x2a05);

    auto index = itemId - 1278; // This will be 1-12
    auto intObj = System::String::ConvertToStringObject(System::Int32_klass::getClass(), &index);

    {
        Dpr::EvScript::EvDataManager::Object* evDataManager = Dpr::EvScript::EvDataManager::get_Instanse();
        if (fromBag)
        {
            System::String::Object * labelName = System::String::Format(System::String::Create("ev_custom_key_item_{0:D1}_bag"), intObj);
            evDataManager->JumpLabel(labelName, nullptr);
        }
        else
        {
            System::String::Object * labelName = System::String::Format(System::String::Create("ev_custom_key_item_{0:D1}"), intObj);
            evDataManager->JumpLabel(labelName, nullptr);
        }
    }
}