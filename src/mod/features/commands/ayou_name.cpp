#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "memory/json.h"
#include "helpers/fsHelper.h"
#include "externals/Dpr/UI/SoftwareKeyboard.h"

const nn::string filePath = "SaveData:/AYou.bin";

void OnComplete(Dpr::EvScript::EvDataManager::DisplayClass773_0::Object* __this, bool isSuccess,
                System::String::Object* resultText) {
    if (!isSuccess) {
        return;
    }

    if (System::String::IsNullOrEmpty(resultText)) {
        Logger::log("[SetAYouName] String is null or empty.\n");
        return;
    }

    nn::json wholeJson = {
            {"AYou", {
                    {"codeName", resultText->asCString()}
            }}
    };

    nn::string writeString = wholeJson.dump(4);
    FsHelper::writeFileToPath((void*) writeString.data(), writeString.size(), filePath.c_str());
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

    Logger::log("[SetAYouName] Opening Keyboard\n");
    Dpr::UI::SoftwareKeyboard::Open(swKeyboardParam, onInputCheck, onComplete);

    return true;
}


bool AYouName(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_AYOU_NAME\n");
    system_load_typeinfo(0x44ac);

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2) {
        auto tagIndex = GetWorkOrIntValue(args->m_Items[1]);

        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
        Dpr::Message::MessageWordSetHelper::SetStringWord(tagIndex, GetAYouName());

    }

    return true;
}