#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UIInputController.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/Dpr/MsgWindow/MsgWindow.h"
#include "externals/Dpr/MsgWindow/MsgWindowParam.h"
#include "externals/System/Collections/IEnumerator.h"
#include "externals/UnityEngine/Animator.h"
#include "externals/Dpr/UI/UIManager.h"

namespace Dpr::UI {
    struct UIWindow : ILClass<UIWindow> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            Dpr::UI::UIInputController::Object* _input;
            void* _canvas;
            UnityEngine::Animator::Object* _animator;
            void* _animEvent;
            int32_t _transitionFadeType;
            int32_t _prevWindowId;
            Dpr::MsgWindow::MsgWindow::Object* _messageWindow;
            Dpr::UI::UIManager::UIInstance::Object* instance;
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

        inline System::Collections::IEnumerator::Object* OpPlayOpenWindowAnimation(
                int32_t prevWindowId, UnityEngine::Events::UnityAction::Object* onOpen) {
            return external<System::Collections::IEnumerator::Object*>(0x01a35390, this, prevWindowId, onOpen);
        }

        inline System::Collections::IEnumerator::Object* OpPlayCloseWindowAnimationAndWaiting(int32_t nextWindowId) {
            return external<System::Collections::IEnumerator::Object*>(0x01a35530, this, nextWindowId);
        }

        inline Dpr::UI::ContextMenuWindow* CreateContextMenuYesNo(void* onClicked, uint32_t SeDecide) {
            return external<Dpr::UI::ContextMenuWindow*>(0x01a35c50, this, onClicked, SeDecide);
        }

        inline void OpenMessageWindow(Dpr::MsgWindow::MsgWindowParam::Object* messageParam) {
            external<void>(0x01a358b0, this, messageParam);
        }

        inline void CloseMessageWindow() {
            external<void>(0x01a359c0, this);
        }
    };
}
