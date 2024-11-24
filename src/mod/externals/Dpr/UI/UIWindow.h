#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UIInputController.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::UI {
    struct UIWindow : ILClass<UIWindow> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            Dpr::UI::UIInputController::Object* _input;
            void* _canvas;
            void* _animator;
            void* _animEvent;
            int32_t _transitionFadeType;
            int32_t _prevWindowId;
            void* _messageWindow;
            void* instance;
            UnityEngine::Events::UnityAction::Object* onClosed;
            UnityEngine::Events::UnityAction::Object* onPreClose;
            bool _IsClosing_k__BackingField;
            int32_t _animStateIn;
            int32_t _animStateOut;
        };

        inline bool IsPushButton(int32_t button, bool isForce) {
            return external<bool>(0x01a352a0, this, button, isForce);
        }

        inline bool IsRepeatButton(int32_t button, bool isForce) {
            return external<bool>(0x01a352c0, this, button, isForce);
        }

        inline void OnOpen(int32_t prevWindowId) {
            external<void>(0x01a351e0, this, prevWindowId);
        }
    };
}
