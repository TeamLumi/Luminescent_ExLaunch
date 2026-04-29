#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Func.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Events/UnityAction.h"

namespace Dpr::UI {
    struct SoftwareKeyboard : ILClass<SoftwareKeyboard> {
        struct Param : ILClass<Param, 0x04c5ee00> {
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
                external<void>(0x0184abe0, this);
            }
        };

        struct Fields {
        };

        static inline int32_t LanguageMaxLength(int32_t maxLength, int32_t langId) {
            return external<int32_t>(0x0184a650, maxLength, langId);
        }

        static inline System::String::Object* GetMessageText(System::String::Object* messageLabel) {
            return external<System::String::Object*>(0x018485e0, messageLabel);
        }

        static inline System::ValueTuple2$$Bool$$String InputCheck(System::String::Object* resultText, int32_t errorState) {
            return external<System::ValueTuple2$$Bool$$String>(0x0184a710, resultText, errorState);
        }

        static inline bool Open(Dpr::UI::SoftwareKeyboard::Param::Object* param, System::Func::Object* onInputCheck, UnityEngine::Events::UnityAction::Object* onComplete) {
            return external<bool>(0x01848e70, param, onInputCheck, onComplete);
        }
    };
}