#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/MsgWindow/MsgWindow.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"

namespace Dpr::MsgWindow {
    struct MsgWindowManager : ILClass<MsgWindowManager> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            // TODO
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
