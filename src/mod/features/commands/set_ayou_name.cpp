#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/SoftwareKeyboard.h"

#include "logger/logger.h"
#include "save/save.h"

void OnComplete(Dpr::EvScript::EvDataManager::DisplayClass773_0::Object* __this, bool isSuccess,
                System::String::Object* resultText) {
    if (!isSuccess) {
        return;
    }

    if (System::String::IsNullOrEmpty(resultText)) {
        Logger::log("[SetAYouName] String is null or empty.\n");
        return;
    }

    getCustomSaveData()->ayou.name = resultText->asCString();
}

bool SetAYouName(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_SET_AYOU_NAME\n");
    system_load_typeinfo(0x4469);

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

    MethodInfo* onCompleteMI = (*Dpr::EvScript::EvDataManager::Method$$EvDataManager_EvCmdNameInPoke_OnComplete)->copyWith((Il2CppMethodPointer)&OnComplete);
    UnityEngine::Events::UnityAction::Object* onComplete = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::bool_String_TypeInfo)->newInstance(dispClass773, onCompleteMI);

    Dpr::UI::SoftwareKeyboard::Open(swKeyboardParam, onInputCheck, onComplete);

    return true;
}