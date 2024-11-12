#include <cstring>

#include "exlaunch.hpp"

#include "memory/vector.h"

#include "data/incense_slots.h"
#include "data/utils.h"
#include "externals/ContextMenuID.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/ShopBoutique.h"
#include "externals/Dpr/UI/ShopBoutiqueChange.h"
#include "externals/Dpr/UI/UIBag.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/Collections/Generic/List.h"

#include "logger/logger.h"

static Dpr::UI::UIBag::__c__DisplayClass127_0::Object* sDisplayClassLocals = nullptr;

void OnSelectedIncenseBurner(Dpr::UI::UIBag::__c__DisplayClass144_0::Object * __this, int32_t selectContextMenuId)
{
    Dpr::UI::UIBag::Object* uiBag = __this->fields.__4__this;
    uiBag->fields.msgWindowController->CloseMsgWindow();
    System::String::Object* labelName;

    switch (selectContextMenuId)
    {
        case (int32_t)ContextMenuID::INCENSE_DISABLE:
            PlayerWork::SetInt((int32_t)FlagWork_Work::WK_INCENSE_SLOT, array_index(INCENSE_SLOTS, "Disable"));
            labelName = System::String::Create("SS_bag_369");
            break;

        case (int32_t)ContextMenuID::INCENSE_UNOVA:
            PlayerWork::SetInt((int32_t)FlagWork_Work::WK_INCENSE_SLOT, array_index(INCENSE_SLOTS, "Unova"));
            labelName = System::String::Create("SS_bag_370");
            break;

        case (int32_t)ContextMenuID::INCENSE_KALOS:
            PlayerWork::SetInt((int32_t)FlagWork_Work::WK_INCENSE_SLOT, array_index(INCENSE_SLOTS, "Kalos"));
            labelName = System::String::Create("SS_bag_370");
            break;

        case (int32_t)ContextMenuID::INCENSE_ALOLA:
            PlayerWork::SetInt((int32_t)FlagWork_Work::WK_INCENSE_SLOT, array_index(INCENSE_SLOTS, "Alola"));
            labelName = System::String::Create("SS_bag_370");
            break;

        case (int32_t)ContextMenuID::INCENSE_HISUI:
            PlayerWork::SetInt((int32_t)FlagWork_Work::WK_INCENSE_SLOT, array_index(INCENSE_SLOTS, "Hisui"));
            labelName = System::String::Create("SS_bag_370");
            break;

        case (int32_t)ContextMenuID::INCENSE_GALAR:
            PlayerWork::SetInt((int32_t)FlagWork_Work::WK_INCENSE_SLOT, array_index(INCENSE_SLOTS, "Galar"));
            labelName = System::String::Create("SS_bag_370");
            break;

        case (int32_t)ContextMenuID::INCENSE_CANCEL:
        default:
            // Cancel, just exit out.
            return;
    }

    system_load_typeinfo(0x955c);
    System::Action::Object* action = System::Action::getClass(System::Action::void_TypeInfo)->newInstance(sDisplayClassLocals, *Dpr::UI::UIBag::__c__DisplayClass127_0::Method$$ShowItemContextMenu_EndUseAction);

    uiBag->fields.msgWindowController->OpenMsgWindow(0, labelName, true, false, nullptr, action);
}

void UseIncenseBurner(int32_t itemId, bool fromBag, Dpr::UI::UIBag::__c__DisplayClass127_1::Object* bagDisplayClass)
{
    Dpr::EvScript::EvDataManager::Object* evDataManager = Dpr::EvScript::EvDataManager::get_Instanse();

    if (fromBag)
    {
        system_load_typeinfo(0x9a2e);
        sDisplayClassLocals = bagDisplayClass->fields.CS___8__locals1;

        Dpr::UI::UIBag::Object * uiBag = bagDisplayClass->fields.CS___8__locals1->fields.__4__this;
        Dpr::UI::UIBag::__c__DisplayClass144_0::Object * displayClass144 = Dpr::UI::UIBag::__c__DisplayClass144_0::newInstance();
        displayClass144->fields.__4__this = uiBag;

        // Build the menu so that you have every option except what you have currently selected.
        auto contextMenuIDVector = nn::vector<int32_t>();
        int32_t incenseSlot = PlayerWork::GetInt((int32_t)FlagWork_Work::WK_INCENSE_SLOT);
        if (incenseSlot != 0)
        {
            contextMenuIDVector.push_back((int32_t)ContextMenuID::INCENSE_DISABLE);
        }
        for (int i=1; i<=5; i++)
        {
            bool flowerEnabled = PlayerWork::GetBool(((int32_t)FlagWork_Flag::FLAG_INCENSE_FLOWER_UNOVA) + i - 1);
            if (i == incenseSlot || !flowerEnabled)
            {
                continue;
            }
            contextMenuIDVector.push_back((int32_t)ContextMenuID::INCENSE_DISABLE + i);
        }
        contextMenuIDVector.push_back((int32_t)ContextMenuID::INCENSE_CANCEL);

        // Create the Method Info for selection in the context menu
        MethodInfo* mi = Dpr::UI::UIBag::__c__DisplayClass144_0::getMethod$$OnSelectedIncenseBurner((Il2CppMethodPointer)&OnSelectedIncenseBurner);
        System::Action::Object* onSelected = System::Action::getClass(System::Action::ContextMenuID_TypeInfo)->newInstance(displayClass144, mi);

        UnityEngine::Vector2::Object pivot = (UnityEngine::Vector2::Object) {
            .fields = {
                .x = 0.5,
                .y = 0.5,
            }
        };

        UnityEngine::Vector3::Object pos = (UnityEngine::Vector3::Object) {
            .fields = {
                .x = 1040.0,
                .y = 360.0,
                .z = 0.0
            }
        };
        
        // There's probably a way to generalize converting a vector to our IL Arrays and then add the method to il2cpp-api.h, but I'm not smart enough to figure it out
        auto contextMenuIDArray = System::Int32_array::newArray(contextMenuIDVector.size());
        std::memcpy(contextMenuIDArray->m_Items, &contextMenuIDVector[0], sizeof(int32_t) * contextMenuIDVector.size());
        
        uiBag->OpenContextMenu(contextMenuIDArray, onSelected, pivot, pos, nullptr, false, false);
        System::String::Object* SS_bag_368 = System::String::Create("SS_bag_368");
        uiBag->fields.msgWindowController->OpenMsgWindow(0, SS_bag_368, false, true, nullptr, nullptr);
    }
    else
    {
        System::String::Object* labelName = System::String::Create("ev_keyitem_incenseburner");
        evDataManager->JumpLabel(labelName, nullptr);
    }
}
