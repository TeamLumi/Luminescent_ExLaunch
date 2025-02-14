#pragma once

#include "externals/il2cpp.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/System/Action.h"
#include "externals/System/Nullable.h"
#include "externals/System/String.h"

namespace Dpr::UI {
    struct UIMsgWindowController : ILClass<UIMsgWindowController, 0x04c5e6a8> {
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
            external<void>(0x01a140a0, this);
        }
        
        inline void OpenMsgWindow(int32_t messageFileType, System::String::Object * labelName, bool isWait, bool isBatchDisplay, System::Action::Object * onFinishedMessage, System::Action::Object * onCloseWindow) {
            external<void>(0x01a133c0, this, messageFileType, labelName, isWait, isBatchDisplay, onFinishedMessage, onCloseWindow);
        }

        inline void CloseMsgWindow() {
            external<void>(0x01a13610, this);
        }

        inline bool OnUpdate(float deltaTime) {
            return external<bool>(0x01a13de0, this, deltaTime);
        }
    };
}



