#pragma once

#include "externals/il2cpp.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/System/Action.h"
#include "externals/System/Nullable.h"
#include "externals/System/String.h"

namespace Dpr::UI {
    struct UIMsgWindowController : ILClass<UIMsgWindowController, game::Address::Dpr_UI_UIMsgWindowController_TypeInfo> {
        struct Fields {
            void* msgWindow;
            void* onCloseMsgWindow;
            void* onFinishedContinueMessage;
            bool isWaitMsgWindow;
            System::Nullable<UnityEngine::Vector2>::Object windowAnchorPosition;
            void* msgWindowParam;
            void* continueMessageLabels;
            void* continueMessageOnSets;
        };

        inline void ctor() {
            external<void>(game::Address::Dpr_UI_UIMsgWindowController_ctor, this);
        }
        
        inline void OpenMsgWindow(int32_t messageFileType, System::String::Object * labelName, bool isWait, bool isBatchDisplay, System::Action::Object * onFinishedMessage, System::Action::Object * onCloseWindow) {
            external<void>(game::Address::Dpr_UI_UIMsgWindowController_OpenMsgWindow, this, messageFileType, labelName, isWait, isBatchDisplay, onFinishedMessage, onCloseWindow);
        }

        inline void CloseMsgWindow() {
            external<void>(game::Address::Dpr_UI_UIMsgWindowController_CloseMsgWindow, this);
        }

        inline bool OnUpdate(float deltaTime) {
            return external<bool>(game::Address::Dpr_UI_UIMsgWindowController_OnUpdate, this, deltaTime);
        }
    };
}



