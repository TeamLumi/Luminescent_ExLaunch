#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"

namespace Dpr::MsgWindow {
    struct MsgWindowManager : ILClass<MsgWindowManager> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            // TODO
        };

        static inline bool get_IsOpenWindow() {
            return external<bool>(0x01dd9810);
        }

        static inline bool get_IsOpen() {
            return external<bool>(0x01dd9910);
        }
    };
}
