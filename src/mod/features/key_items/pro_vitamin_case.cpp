#include "exlaunch.hpp"
#include "memory/vector.h"
#include "romdata/romdata.h"

#include "externals/Audio/AudioManager.h"
#include "externals/ContextMenuID.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/UIBag.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/BoxWindow.h"
#include "externals/Dpr/UI/SoftwareKeyboard.h"
#include "externals/PlayerWork.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/Int32Class.h"

static Dpr::UI::UIBag::__c__DisplayClass127_0::Object* sDisplayClassLocals = nullptr;
static Pml::PokePara::PokemonParam::Object* param;
static Pml::PokePara::CoreParam* coreParam;
static int32_t stat = -1;

const uint32_t PROVITAMIN_SOUND_ID = 0x45a3539d;

void OpenProVitaminKeyboard(Dpr::EvScript::EvDataManager::DisplayClass831_0::Object* __this, bool isSuccess, System::String::Object* resultText) {
    if (isSuccess && !System::String::IsNullOrEmpty(resultText)) {

        int32_t resultNumber = 0;
        System::Int32Class::TryParse(resultText, &resultNumber);

        if (stat == 0) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::HP, resultNumber);}
        if (stat == 1) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::ATK, resultNumber);}
        if (stat == 2) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::DEF, resultNumber);}
        if (stat == 3) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::SPATK, resultNumber);}
        if (stat == 4) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::SPDEF, resultNumber);}
        if (stat == 5) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::AGI, resultNumber);}

        Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
        Dpr::EvScript::EvDataManager::get_Instanse()->fields._softwareKeyboardSubState = 0; // Done
    }
    else {
        Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
        Dpr::EvScript::EvDataManager::get_Instanse()->fields._softwareKeyboardSubState = 2; // Error
    }
}

void SetupProVitaminKeyboard(Dpr::EvScript::EvDataManager::Object* manager)
{
    system_load_typeinfo(0x43e0);

    manager->fields._softwareKeyboardSubState = 1; // In progress

    auto dispClass831 = Dpr::EvScript::EvDataManager::DisplayClass831_0::newInstance();
    auto swKeyboardParam = Dpr::UI::SoftwareKeyboard::Param::newInstance();

    swKeyboardParam->fields.keyboardMode = 1;
    swKeyboardParam->fields.text = System::String::Create("");
    swKeyboardParam->fields.textMinLength = 1;
    swKeyboardParam->fields.textMaxLength = 3;
    swKeyboardParam->fields.headerText = System::String::Create("Input the new Effort Value for this stat:");
    swKeyboardParam->fields.subText = nullptr;
    swKeyboardParam->fields.guideText = nullptr;
    swKeyboardParam->fields.okText = nullptr;
    swKeyboardParam->fields.invalidCharFlag = 4;

    MethodInfo* onInputCheckMI = *Dpr::EvScript::EvDataManager::Method$$EvDataManager_EvCmdBirthDayInput_OnInputCheck;
    System::Func::Object* onInputCheck = System::Func::getClass(System::Func::String__SoftwareKeyboard_ErrorState__ValueTuple_bool_String__TypeInfo)->newInstance(manager, onInputCheckMI);

    MethodInfo* onCompletedMI = Dpr::EvScript::EvDataManager::getMethod$$EvCmdBirthDayInput_OnCompleteCustomNumberInput((Il2CppMethodPointer)&OpenProVitaminKeyboard);
    UnityEngine::Events::UnityAction::Object* onCompleted = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::bool_String_TypeInfo)->newInstance(dispClass831, onCompletedMI);

    Dpr::UI::SoftwareKeyboard::Open(swKeyboardParam, onInputCheck, onCompleted);

}

void OnSelectedProVitaminCase(Dpr::UI::UIBag::__c__DisplayClass144_0::Object * __this, int32_t selectContextMenuId)
{
    Dpr::UI::UIBag::Object* uiBag = __this->fields.__4__this;
    uiBag->fields.msgWindowController->CloseMsgWindow();
    System::String::Object* labelName;

    Dpr::EvScript::EvDataManager::Object* manager = Dpr::EvScript::EvDataManager::get_Instanse();
    param = manager->GetPokemonParam(-1, 0);
    coreParam = (Pml::PokePara::CoreParam *)param;

    switch (selectContextMenuId)
    {
        case (int32_t)ContextMenuID::PRO_VITAMIN_HP:
            stat = 0;
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_ATTACK:
            stat = 1;
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_DEFENSE:
            stat = 2;
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_SPECIAL_ATTACK:
            stat = 3;
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_SPECIAL_DEFENSE:
            stat = 4;
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_SPEED:
            stat = 5;
            break;

        case (int32_t)ContextMenuID::CANCEL:
        default:
            // Cancel, just exit out.
            return;
    }

    SetupProVitaminKeyboard(manager);

    system_load_typeinfo(0x955c);
    System::Action::Object* action = System::Action::getClass(System::Action::void_TypeInfo)->newInstance(sDisplayClassLocals, *Dpr::UI::UIBag::__c__DisplayClass127_0::Method$$ShowItemContextMenu_EndUseAction);


    SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
    Audio::AudioManager::Object * audioManager = Audio::AudioManager::get_Instance();
    audioManager->PlaySe(PROVITAMIN_SOUND_ID, nullptr);

    labelName = System::String::Create("SS_bag_374");
    uiBag->fields.msgWindowController->OpenMsgWindow(0, labelName, true, false, nullptr, action);
}

void OpenBoxParam(Dpr::UI::BoxWindow::Object* __this, int32_t openType, System::Action::Object* onSelected, int32_t prevWindowId) {
    system_load_typeinfo(0x2672);

    auto openParam = Dpr::UI::BoxWindow::OpenParam::newInstance();

    Dpr::UI::UIManager::getClass()->initIfNeeded();
    auto boxOpenParam = Dpr::UI::UIManager::get_Instance()->GetBoxOpenData(openType);

    openParam->fields.openType = 0;
    openParam->fields.selectCount = 1;
    openParam->fields.targetLevel = 0;
    openParam->fields.isEnableDying = true;
    openParam->fields.isEnableEgg = false;
    openParam->fields.isEnableParty = true;
    openParam->fields.targetsPokeNo = boxOpenParam->fields.MonsNo->m_Items[0] != 0 ? boxOpenParam->fields.MonsNo : nullptr;

    __this->Open(openParam, onSelected, prevWindowId);
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
        Dpr::EvScript::EvDataManager::Object* manager = Dpr::EvScript::EvDataManager::get_Instanse();
        System::String::Object* labelName = System::String::Create("ev_keyitem_pro_vitamin_case");
        manager->JumpLabel(labelName, nullptr);
    }
}