#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"

namespace Dpr::MsgWindow {
    struct MsgWindowManager : ILClass<MsgWindowManager> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            // TODO
        };

        static inline bool get_IsOpenWindow() {
            return external<bool>(game::Address::Dpr_MsgWindow_MsgWindowManager_get_IsOpenWindow);
        }

        static inline bool get_IsOpen() {
            return external<bool>(game::Address::Dpr_MsgWindow_MsgWindowManager_get_IsOpen);
        }
    };
}
