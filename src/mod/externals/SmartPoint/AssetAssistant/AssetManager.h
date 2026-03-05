#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"

namespace SmartPoint::AssetAssistant {
    struct AssetManager : ILClass<AssetManager, 0x04c587b8> {
        struct Fields {
            // TODO
        };

        struct StaticFields {
            // TODO
        };

        static inline int32_t UnloadAssetBundle(System::String::Object* assetBundleName) {
            return external<int32_t>(0x022dbde0, assetBundleName);
        }
    };
}
