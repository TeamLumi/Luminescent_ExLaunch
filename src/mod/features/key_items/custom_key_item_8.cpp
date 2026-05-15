#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/UIBag.h"
#include "externals/Dpr/UI/UIManager.h"

void UseCustomKeyItem8(int32_t itemId, bool fromBag, Dpr::UI::UIBag::__c__DisplayClass127_1::Object* bagDisplayClass)
{
    Dpr::EvScript::EvDataManager::Object* evDataManager = Dpr::EvScript::EvDataManager::get_Instanse();
    if (fromBag)
    {
        System::String::Object * labelName = System::String::Create("ev_custom_key_item_8_bag");
        evDataManager->JumpLabel(labelName, nullptr);
    }
    else
    {
        System::String::Object * labelName = System::String::Create("ev_custom_key_item_8");
        evDataManager->JumpLabel(labelName, nullptr);
    }
}