#pragma once
#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/RectTransform.h"

namespace Dpr::MsgWindow {
    struct MsgWindow : ILClass<MsgWindow> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            bool autoResize;
            void* config;
            void* windowMessage;
            void* uiContents;
            void* windowAnimator;
            void* dataModel;
            void* soundPlayer;
            void* wndInput;
            UnityEngine::RectTransform::Object* contextMenuRect;
            void* waitAutoTimer;
            bool bInitialize;
            bool bIsAutoMessage;
        };

        inline bool get_isEnabledInput() {
            return external<bool>(0x01face60, this);
        };
    };
}