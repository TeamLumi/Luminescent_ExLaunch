#include "romdata/romdata.h"
#include "externals/ContextMenuID.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/UIBag.h"

void UseProVitaminCase(int32_t itemId, bool fromBag, Dpr::UI::UIBag::__c__DisplayClass127_1::Object* bagDisplayClass)
{
    Dpr::EvScript::EvDataManager::Object* manager = Dpr::EvScript::EvDataManager::get_Instanse();
    {
        if (fromBag)
        {
            Dpr::UI::UIBag::Object* bag = bagDisplayClass->fields.CS___8__locals1->fields.__4__this;
            bag->Close(bag->fields.onClosed, bag->fields._prevWindowId, true);

            System::String::Object* labelName = System::String::Create("ev_keyitem_pro_vitamin_case_bag");
            manager->JumpLabel(labelName, nullptr);
        }
        else
        {
            System::String::Object* labelName = System::String::Create("ev_keyitem_pro_vitamin_case");
            manager->JumpLabel(labelName, nullptr);
        }
    }
}