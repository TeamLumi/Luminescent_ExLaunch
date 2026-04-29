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
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/Int32Class.h"

static Dpr::UI::UIBag::__c__DisplayClass127_0::Object* sDisplayClassLocals = nullptr;
static Pml::PokePara::CoreParam* coreParam;
static int32_t stat = -1;

const uint32_t PROVITAMIN_SOUND_ID = 0x45a3539d;

System::ValueTuple2$$Bool$$String CheckProVitaminInput(Dpr::EvScript::EvDataManager::DisplayClass831_0::Object* __this, System::String::Object* resultText, int32_t errorState)
{
    system_load_typeinfo(0x9896);

    auto inputCheck = Dpr::UI::SoftwareKeyboard::InputCheck(resultText, errorState);

    if (!inputCheck.fields.Item1) {
        return inputCheck;
    }

    int32_t resultNumber;
    int32_t maxValue = 252;
    bool parsed = System::Int32Class::TryParse(resultText, &resultNumber);

    if (!parsed || resultNumber > maxValue) {
        inputCheck.fields.Item1 = false;
        inputCheck.fields.Item2 = Dpr::UI::SoftwareKeyboard::GetMessageText(System::String::Create("SS_strinput_041"));
        return inputCheck;
    }

    inputCheck.fields.Item2 = nullptr;
    return inputCheck;
}

void CompleteProVitaminKeyboard(Dpr::EvScript::EvDataManager::DisplayClass831_0::Object* __this, bool isSuccess, System::String::Object* resultText) {

    if (isSuccess && !System::String::IsNullOrEmpty(resultText)) {

        int32_t resultNumber = 0;
        System::Int32Class::TryParse(resultText, &resultNumber);

        Logger::log("Calling ChangeEffortPower with coreParam, stat %d and resultNumber %d\n", stat, resultNumber);

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

        Logger::log("Keyboard input error occured.\n");

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
    swKeyboardParam->fields.headerText = Dpr::UI::SoftwareKeyboard::GetMessageText(System::String::Create("SS_strinput_pro_vitamin"));
    swKeyboardParam->fields.subText = nullptr;
    swKeyboardParam->fields.guideText = nullptr;
    swKeyboardParam->fields.okText = nullptr;
    swKeyboardParam->fields.invalidCharFlag = 4;

    MethodInfo* onInputCheckMI = Dpr::EvScript::EvDataManager::getMethod$$EvCmdBirthDayInput_CheckProVitaminInput((Il2CppMethodPointer)&CheckProVitaminInput);
    System::Func::Object* onInputCheck = System::Func::getClass(System::Func::String__SoftwareKeyboard_ErrorState__ValueTuple_bool_String__TypeInfo)->newInstance(dispClass831, onInputCheckMI);

    MethodInfo* onCompleteMI = Dpr::EvScript::EvDataManager::getMethod$$EvCmdBirthDayInput_CompleteProVitaminKeyboard((Il2CppMethodPointer)&CompleteProVitaminKeyboard);
    UnityEngine::Events::UnityAction::Object* onComplete = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::bool_String_TypeInfo)->newInstance(dispClass831, onCompleteMI);

    Dpr::UI::SoftwareKeyboard::Open(swKeyboardParam, onInputCheck, onComplete);
}

void OnSelectedProVitaminCase(Dpr::UI::UIBag::__c__DisplayClass144_0::Object * __this, int32_t selectContextMenuId)
{
    Dpr::UI::UIBag::Object* uiBag = __this->fields.__4__this;
    uiBag->fields.msgWindowController->CloseMsgWindow();
    System::String::Object* labelName;

    Dpr::EvScript::EvDataManager::Object* manager = Dpr::EvScript::EvDataManager::get_Instanse();

    switch (selectContextMenuId)
    {
        case (int32_t)ContextMenuID::PRO_VITAMIN_HP:
            stat = 0;
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_ATK:
            stat = 1;
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_DEF:
            stat = 2;
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_SPATK:
            stat = 3;
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_SPDEF:
            stat = 4;
            break;

        case (int32_t)ContextMenuID::PRO_VITAMIN_AGI:
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

void BuildContextMenu(Dpr::UI::UIBag::Object* __this, Dpr::UI::PokemonPartyItem::Object* pokemonPartyItem, int32_t index)
{
        system_load_typeinfo(0x9a2e);

        Dpr::EvScript::EvDataManager::Object* manager = Dpr::EvScript::EvDataManager::get_Instanse();

        Logger::log("Getting coreParam of the selected Pokemon with trayIndex -1 and index %d\n", index);

        Pml::PokePara::PokemonParam::Object* param = manager->GetPokemonParam(-1, index);
        coreParam = (Pml::PokePara::CoreParam *)param;

        Dpr::UI::UIBag::Object * uiBag = __this;
        Dpr::UI::UIBag::__c__DisplayClass144_0::Object * displayClass144 = Dpr::UI::UIBag::__c__DisplayClass144_0::newInstance();
        displayClass144->fields.__4__this = uiBag;

        // Build the menu so that you have every option.
        auto contextMenuIDVector = nn::vector<int32_t>();
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_PROVITAMIN_HP)) {contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_HP);}
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_PROVITAMIN_ATK)) {contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_ATK);}
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_PROVITAMIN_DEF)) {contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_DEF);}
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_PROVITAMIN_SPATK)) {contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_SPATK);}
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_PROVITAMIN_SPDEF)) {contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_SPDEF);}
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_PROVITAMIN_AGI)) {contextMenuIDVector.push_back((int32_t)ContextMenuID::PRO_VITAMIN_AGI);}
        contextMenuIDVector.push_back((int32_t)ContextMenuID::CANCEL);

        // Create the Method Info for selection in the context menu
        MethodInfo* mi144 = Dpr::UI::UIBag::__c__DisplayClass144_0::getMethod$$OnSelectedProVitaminCase((Il2CppMethodPointer)&OnSelectedProVitaminCase);
        System::Action::Object* onSelected = System::Action::getClass(System::Action::ContextMenuID_TypeInfo)->newInstance(displayClass144, mi144);

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

void UseProVitaminCase(int32_t itemId, bool fromBag, Dpr::UI::UIBag::__c__DisplayClass127_1::Object* bagDisplayClass)
{
    if (fromBag)
    {
        system_load_typeinfo(0x9a0a);
        sDisplayClassLocals = bagDisplayClass->fields.CS___8__locals1;

        Dpr::UI::UIBag::Object* uiBagParty = sDisplayClassLocals->fields.__4__this;

        System::String::Object* SS_bag_375 = System::String::Create("SS_bag_375");
        uiBagParty->fields.msgWindowController->OpenMsgWindow(0, SS_bag_375, false, false, nullptr, nullptr);

        MethodInfo* mi127 = Dpr::UI::UIBag::getMethod$$BuildContextMenu((Il2CppMethodPointer)&BuildContextMenu);

        UnityEngine::Events::UnityAction::Object* onClicked = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::PokemonPartyItem__int_TypeInfo)->newInstance(uiBagParty, mi127);

        uiBagParty->StartSelectPokemonParty(onClicked);
    }
    else
    {
        Dpr::EvScript::EvDataManager::Object* manager = Dpr::EvScript::EvDataManager::get_Instanse();
        System::String::Object* labelName = System::String::Create("ev_keyitem_pro_vitamin_case");
        manager->JumpLabel(labelName, nullptr);
    }
}