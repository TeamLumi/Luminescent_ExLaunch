#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"

namespace Dpr::NetworkUtils {
    struct NetworkManager : ILClass<NetworkManager, 0x04c59da0> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            // TODO
        };

        static inline bool IsShowApplicationErrorDialog() {
            return external<bool>(0x01deb4d0);
        }
    };
}
