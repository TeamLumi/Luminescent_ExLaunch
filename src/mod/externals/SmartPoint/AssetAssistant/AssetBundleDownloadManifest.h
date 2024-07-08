#pragma once

#include "externals/il2cpp-api.h"
#include "externals/SmartPoint/AssetAssistant/AssetBundleRecord.h"

namespace SmartPoint::AssetAssistant {
    struct AssetBundleDownloadManifest : ILClass<AssetBundleDownloadManifest> {
        struct Fields {
            int32_t _version;
            System::String::Object* _projectName;
            AssetBundleRecord::Array* _records;
            System::String::Array* _assetBundleNamesWithVariant;
            void* _variantMap;
            void* _recordLookupFromAssetBundleName;
            void* _recordLookupFromAssetPath;
            bool _dirty;
            System::String::Object* _path;
            AssetBundleDownloadManifest::Object* _latest;
        };
    };
}