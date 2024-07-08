#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace SmartPoint::AssetAssistant {
    struct AssetBundleRecord : ILClass<AssetBundleRecord, 0x04c58808> {
        struct Fields {
            System::String::Object* projectName;
            System::String::Object* assetBundleName;
            void* hash;
            void* lastWriteTime;
            bool isStreamingSceneAssetBundle;
            System::String::Array* allDependencies;
            System::String::Array* assetPaths;
            int64_t size;
            AssetBundleRecord::Object* latest;
            bool isBeginInstalled;
            bool isSimulation;
        };

        inline void ctor(System::String::Object* _projectName, System::String::Object* _assetBundleName) {
            external<void>(0x022dad10, this, _projectName, _assetBundleName, System::Collections::Generic::Dictionary$$String__AssetBundleRecord::Method$$TryGetValue);
        }
    };
}
