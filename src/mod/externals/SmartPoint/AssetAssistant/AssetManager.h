#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/AssetRequestOperation.h"
#include "externals/System/String.h"

namespace SmartPoint::AssetAssistant {
    struct AssetManager : ILClass<AssetManager, game::Address::SmartPoint_AssetAssistant_AssetManager_TypeInfo> {
        struct Fields {
            // TODO
        };

        struct StaticFields {
            // TODO
        };

        static inline AssetRequestOperation::Object* AppendAssetBundleRequest(System::String::Object* assetBundleName, bool loadAllAssets, void* callback, System::String::Array* variants) {
            return external<AssetRequestOperation::Object *>(game::Address::SmartPoint_AssetAssistant_AssetManager_AppendAssetBundleRequest, assetBundleName, loadAllAssets, callback, variants);
        }

        static inline int32_t UnloadAssetBundle(System::String::Object* assetBundleName) {
            return external<int32_t>(game::Address::SmartPoint_AssetAssistant_AssetManager_UnloadAssetBundle, assetBundleName);
        }
    };
}
