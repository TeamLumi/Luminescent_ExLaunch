#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/AssetRequestOperation.h"
#include "externals/System/String.h"

namespace SmartPoint::AssetAssistant {
    struct AssetManager : ILClass<AssetManager, 0x04c587b8> {
        struct Fields {
            // TODO
        };

        static inline AssetRequestOperation::Object* AppendAssetBundleRequest(System::String::Object* assetBundleName, bool loadAllAssets, void* callback, System::String::Array* variants) {
            return external<AssetRequestOperation::Object*>(0x022dbfb0, assetBundleName, loadAllAssets, callback, variants);
        }
    };
}
