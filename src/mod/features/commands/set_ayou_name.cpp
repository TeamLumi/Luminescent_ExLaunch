#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/SoftwareKeyboard.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"

void OnComplete(Dpr::EvScript::EvDataManager::DisplayClass773_0::Object* __this, bool isSuccess, System::String::Object* resultText) {
    if (isSuccess && !System::String::IsNullOrEmpty(resultText)) {
        getCustomSaveData()->ayou.name = resultText->asCString();

        Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
        Dpr::EvScript::EvDataManager::get_Instanse()->fields._softwareKeyboardSubState = 0; // Done
    }
    else {
        Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
        Dpr::EvScript::EvDataManager::get_Instanse()->fields._softwareKeyboardSubState = 2; // Error
    }
}

bool SetAYouName(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_SET_AYOU_NAME\n");
    system_load_typeinfo(0x4469);

    manager->fields._softwareKeyboardSubState = 1; // In progress

    auto dispClass773 = Dpr::EvScript::EvDataManager::DisplayClass773_0::newInstance();
    auto swKeyboardParam = Dpr::UI::SoftwareKeyboard::Param::newInstance();

    swKeyboardParam->fields.text = System::String::Create("");
    swKeyboardParam->fields.textMinLength = 1;
    swKeyboardParam->fields.textMaxLength = Dpr::UI::SoftwareKeyboard::LanguageMaxLength(6, -1);
    swKeyboardParam->fields.headerText = System::String::Create("Codename: ");
    swKeyboardParam->fields.subText = nullptr;
    swKeyboardParam->fields.guideText = nullptr;
    swKeyboardParam->fields.okText = nullptr;
    swKeyboardParam->fields.invalidCharFlag = 4;

    MethodInfo* onInputCheckMI = *Dpr::EvScript::EvDataManager::Method$$EvDataManager_EvCmdNameInPoke_OnInputCheck;
    System::Func::Object* onInputCheck = System::Func::getClass(System::Func::String__SoftwareKeyboard_ErrorState__ValueTuple_bool_String__TypeInfo)->newInstance(manager, onInputCheckMI);

    MethodInfo* onCompleteMI = Dpr::EvScript::EvDataManager::getMethod$$EvCmdNameInPoke_OnCompleteAYou((Il2CppMethodPointer)&OnComplete);
    UnityEngine::Events::UnityAction::Object* onComplete = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::bool_String_TypeInfo)->newInstance(dispClass773, onCompleteMI);

    Dpr::UI::SoftwareKeyboard::Open(swKeyboardParam, onInputCheck, onComplete);

    return manager->fields._softwareKeyboardSubState == 0;
}