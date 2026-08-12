#pragma once

#include "MsgWindowParam.h"
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

        static inline Dpr::MsgWindow::MsgWindowParam::Object* OpenMsg(Dpr::MsgWindow::MsgWindowParam::Object* msgWindowParam) {
            return external<Dpr::MsgWindow::MsgWindowParam::Object*>(0x01dd9990, msgWindowParam);
        }
    };
}
