#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/MsgWindow/MsgWindow.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/GameObject.h"

namespace Dpr::MsgWindow {
    struct MsgWindowManager : ILClass<MsgWindowManager> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            void* dataContainer;              // 0x18 MsgWindowDataContainer
            void* msgWindowData;              // 0x20 MsgWindowData
            void* contextMenu;               // 0x28 ContextMenuWindow
            UnityEngine::GameObject::Object* msgWindowObj; // 0x30
            MsgWindow::Object* msgWindow;     // 0x38
            System::String::Array* YES_NO_LABEL_ARRAY; // 0x40
        };

        static inline MsgWindowManager::Object* get_Instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(
                SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$MsgWindowManager$$get_Instance);
        }

        // MsgWindow @ 0x01dd9800
        inline MsgWindow::Object* get_MsgWindow() {
            return external<MsgWindow::Object*>(0x01dd9800, this);
        }

        static inline bool get_IsOpenWindow() {
            return external<bool>(0x01dd9810);
        }

        static inline bool get_IsOpen() {
            return external<bool>(0x01dd9910);
        }
    };
}
