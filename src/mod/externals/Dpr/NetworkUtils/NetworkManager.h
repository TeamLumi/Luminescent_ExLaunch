#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"

namespace Dpr::NetworkUtils {
    struct NetworkManager : ILClass<NetworkManager, game::Address::Dpr_NetworkUtils_NetworkManager_TypeInfo> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            // TODO
        };

        static inline bool IsShowApplicationErrorDialog() {
            return external<bool>(game::Address::Dpr_NetworkUtils_NetworkManager_IsShowApplicationErrorDialog);
        }
    };
}
