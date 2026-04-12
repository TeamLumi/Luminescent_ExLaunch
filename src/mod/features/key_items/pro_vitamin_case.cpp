#include <cstring>
#include <romdata/romdata.h>

#include "exlaunch.hpp"

#include "memory/vector.h"

#include "externals/ContextMenuID.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/ShopBoutique.h"
#include "externals/Dpr/UI/UIBag.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/PlayerWork.h"

static Dpr::UI::UIBag::__c__DisplayClass127_0::Object* sDisplayClassLocals = nullptr;

void OnSelectedProVitaminCase(Dpr::UI::UIBag::__c__DisplayClass144_0::Object * __this, int32_t selectContextMenuId)
{
    Dpr::UI::UIBag::Object* uiBag = __this->fields.__4__this;
    uiBag->fields.msgWindowController->CloseMsgWindow();
    System::String::Object* labelName;

    Dpr::EvScript::EvDataManager::Object* manager = Dpr::EvScript::EvDataManager::get_Instanse();
    Pml::PokePara::PokemonParam::Object* param = manager->GetPokemonParam(-1, 0);
    auto coreparam = (Pml::PokePara::CoreParam *)param;
    int stat;

    switch (selectContextMenuId)
    {
        case (int32_t)ContextMenuID::PRO_VITAMIN_HP:
            labelName = System::String::Create("SS_bag_374");
            stat = coreparam->GetEffortPower(0);
            coreparam->ChangeEffortPower(Pml::PokePara::PowerID::HP, stat + 10);
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_ATTACK:
            labelName = System::String::Create("SS_bag_375");
            stat = coreparam->GetEffortPower(1);
            coreparam->ChangeEffortPower(Pml::PokePara::PowerID::ATK, stat + 10);
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_DEFENSE:
            labelName = System::String::Create("SS_bag_376");
            stat = coreparam->GetEffortPower(2);
            coreparam->ChangeEffortPower(Pml::PokePara::PowerID::DEF, stat + 10);
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_SPECIAL_ATTACK:
            labelName = System::String::Create("SS_bag_377");
            stat = coreparam->GetEffortPower(3);
            coreparam->ChangeEffortPower(Pml::PokePara::PowerID::SPATK, stat + 10);
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_SPECIAL_DEFENSE:
            labelName = System::String::Create("SS_bag_378");
            stat = coreparam->GetEffortPower(4);
            coreparam->ChangeEffortPower(Pml::PokePara::PowerID::SPDEF, stat + 10);
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_SPEED:
            labelName = System::String::Create("SS_bag_379");
            stat = coreparam->GetEffortPower(5);
            coreparam->ChangeEffortPower(Pml::PokePara::PowerID::AGI, stat + 10);
            break;

        case (int32_t)ContextMenuID::CANCEL:
        default:
            // Cancel, just exit out.
            return;
    }

    system_load_typeinfo(0x955c);
    System::Action::Object* action = System::Action::getClass(System::Action::void_TypeInfo)->newInstance(sDisplayClassLocals, *Dpr::UI::UIBag::__c__DisplayClass127_0::Method$$ShowItemContextMenu_EndUseAction);

    uiBag->fields.msgWindowController->OpenMsgWindow(0, labelName, true, false, nullptr, action);
}

void UseProVitaminCase(int32_t itemId, bool fromBag, Dpr::UI::UIBag::__c__DisplayClass127_1::Object* bagDisplayClass)
{
    if (fromBag)
    {
        system_load_typeinfo(0x9a2e);
        sDisplayClassLocals = bagDisplayClass->fields.CS___8__locals1;

        Dpr::UI::UIBag::Object * uiBag = bagDisplayClass->fields.CS___8__locals1->fields.__4__this;
        Dpr::UI::UIBag::__c__DisplayClass144_0::Object * displayClass144 = Dpr::UI::UIBag::__c__DisplayClass144_0::newInstance();
        displayClass144->fields.__4__this = uiBag;

        // Build the menu so that you have every option.
        auto contextMenuIDVector = nn::vector<int32_t>();
        contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_HP);
        contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_ATTACK);
        contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_DEFENSE);
        contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_SPECIAL_ATTACK);
        contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_SPECIAL_DEFENSE);
        contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_SPEED);
        contextMenuIDVector.push_back((int32_t)ContextMenuID::CANCEL);

        // Create the Method Info for selection in the context menu
        MethodInfo* mi = Dpr::UI::UIBag::__c__DisplayClass144_0::getMethod$$OnSelectedProVitaminCase((Il2CppMethodPointer)&OnSelectedProVitaminCase);
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
        System::String::Object* SS_bag_373 = System::String::Create("SS_bag_373");
        uiBag->fields.msgWindowController->OpenMsgWindow(0, SS_bag_373, false, true, nullptr, nullptr);
    }
    else
    {
        //System::String::Object* labelName = System::String::Create("ev_keyitem_incenseburner");
        //evDataManager->JumpLabel(labelName, nullptr);
    }
}
