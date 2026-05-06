#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/SoftwareKeyboard.h"
#include "externals/System/Int32Class.h"
#include "externals/UnityEngine/Mathf.h"

#include "logger/logger.h"
#include "utils/cmd_utils.h"

static int32_t maxValue;

System::ValueTuple2$$Bool$$String CheckCustomNumberInput(Dpr::EvScript::EvDataManager::DisplayClass831_0::Object* __this, System::String::Object* resultText, int32_t errorState) {

    system_load_typeinfo(0x9896);

    auto inputCheck = Dpr::UI::SoftwareKeyboard::InputCheck(resultText, errorState);

    if (!inputCheck.fields.Item1) {
        return inputCheck;
    }

    int32_t resultNumber;
    bool parsed = System::Int32Class::TryParse(resultText, &resultNumber);

    if (!parsed || resultNumber > maxValue) {
        inputCheck.fields.Item1 = false;
        inputCheck.fields.Item2 = Dpr::UI::SoftwareKeyboard::GetMessageText(System::String::Create("SS_strinput_041"));
        return inputCheck;
    }

    inputCheck.fields.Item2 = nullptr;
    return inputCheck;
}

void CompleteCustomNumberInput(Dpr::EvScript::EvDataManager::DisplayClass831_0::Object* __this, bool isSuccess, System::String::Object* resultText) {

    if (isSuccess && !System::String::IsNullOrEmpty(resultText)) {
        Dpr::EvScript::EvDataManager::Object* manager = Dpr::EvScript::EvDataManager::get_Instanse();
        EvData::Aregment::Array* args = manager->fields._evArg;

        int32_t resultNumber = 0;
        bool parsed = System::Int32Class::TryParse(resultText, &resultNumber);
        if (!parsed) {resultNumber = 0;}

        SetWorkToValue(args->m_Items[1], resultNumber);

        Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
        Dpr::EvScript::EvDataManager::get_Instanse()->fields._softwareKeyboardSubState = 0; // Done
    }
    else {
        Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
        Dpr::EvScript::EvDataManager::get_Instanse()->fields._softwareKeyboardSubState = 2; // Error
    }
}

bool CustomNumberInput(Dpr::EvScript::EvDataManager::Object* manager) {

    EvData::Aregment::Array* args = manager->fields._evArg;

    Logger::log("_CUSTOM_NUMBER_INPUT\n");
    system_load_typeinfo(0x43e0);

    manager->fields._softwareKeyboardSubState = 1; // In progress

    auto dispClass831 = Dpr::EvScript::EvDataManager::DisplayClass831_0::newInstance();
    auto swKeyboardParam = Dpr::UI::SoftwareKeyboard::Param::newInstance();

    maxValue = GetWorkOrIntValue(args->m_Items[2]);
    int32_t maxLength = (int32_t)(UnityEngine::Mathf::Log10(maxValue)) + 1;
    System::String::Object* headerLabel = GetStringText(manager,args->m_Items[3]);

    swKeyboardParam->fields.keyboardMode = 1;
    swKeyboardParam->fields.text = System::String::Create("");
    swKeyboardParam->fields.textMinLength = 1;
    swKeyboardParam->fields.textMaxLength = maxLength;
    swKeyboardParam->fields.headerText = Dpr::UI::SoftwareKeyboard::GetMessageText(headerLabel);
    swKeyboardParam->fields.subText = nullptr;
    swKeyboardParam->fields.guideText = nullptr;
    swKeyboardParam->fields.okText = nullptr;
    swKeyboardParam->fields.invalidCharFlag = 4;

    MethodInfo* onInputCheckMI = Dpr::EvScript::EvDataManager::getMethod$$EvCmdBirthDayInput_CheckCustomNumberInput((Il2CppMethodPointer)&CheckCustomNumberInput);
    System::Func::Object* onInputCheck = System::Func::getClass(System::Func::String__SoftwareKeyboard_ErrorState__ValueTuple_bool_String__TypeInfo)->newInstance(dispClass831, onInputCheckMI);

    MethodInfo* onCompleteMI = Dpr::EvScript::EvDataManager::getMethod$$EvCmdBirthDayInput_CompleteCustomNumberInput((Il2CppMethodPointer)&CompleteCustomNumberInput);
    UnityEngine::Events::UnityAction::Object* onComplete = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::bool_String_TypeInfo)->newInstance(dispClass831, onCompleteMI);

    Dpr::UI::SoftwareKeyboard::Open(swKeyboardParam, onInputCheck, onComplete);

    return manager->fields._softwareKeyboardSubState == 0;
}