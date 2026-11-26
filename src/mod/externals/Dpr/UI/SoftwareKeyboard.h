#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Func.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Events/UnityAction.h"

namespace Dpr::UI {
    struct SoftwareKeyboard : ILClass<SoftwareKeyboard> {
        struct Param : ILClass<Param, game::Address::Dpr_UI_SoftwareKeyboard_Param_TypeInfo> {
            struct Fields {
                int32_t preset;
                int32_t keyboardMode;
                bool isPredictionEnabled;
                int32_t invalidCharFlag;
                int32_t initialCursorPos;
                int32_t textMaxLength;
                int32_t textMinLength;
                int32_t passwordMode;
                int32_t inputFormMode;
                bool isUseNewLine;
                bool isUseBlurBackground;
                System::Int32_array* separateTextPos;
                System::String::Object* text;
                System::String::Object* guideText;
                System::String::Object* headerText;
                System::String::Object* subText;
                System::String::Object* okText;
                int32_t disableErrorChecks;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_UI_SoftwareKeyboard_Param_ctor, this);
            }
        };

        struct Fields {
        };

        static inline int32_t LanguageMaxLength(int32_t maxLength, int32_t langId) {
            return external<int32_t>(game::Address::Dpr_UI_SoftwareKeyboard_LanguageMaxLength, maxLength, langId);
        }

        static inline System::String::Object* GetMessageText(System::String::Object* messageLabel) {
            return external<System::String::Object*>(game::Address::Dpr_UI_SoftwareKeyboard_GetMessageText, messageLabel);
        }

        static inline System::ValueTuple2$$Bool$$String::Object InputCheck(System::String::Object* resultText, int32_t errorState) {
            return external<System::ValueTuple2$$Bool$$String::Object>(game::Address::Dpr_UI_SoftwareKeyboard_InputCheck, resultText, errorState);
        }

        static inline bool Open(Dpr::UI::SoftwareKeyboard::Param::Object* param, System::Func::Object* onInputCheck, UnityEngine::Events::UnityAction::Object* onComplete) {
            return external<bool>(game::Address::Dpr_UI_SoftwareKeyboard_Open, param, onInputCheck, onComplete);
        }
    };
}