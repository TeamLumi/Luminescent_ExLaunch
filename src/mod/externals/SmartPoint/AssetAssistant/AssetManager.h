#pragma once

#include "externals/il2cpp-api.h"
#include "externals/SmartPoint/AssetAssistant/AssetBundleDownloadManifest.h"

namespace SmartPoint::AssetAssistant {
    struct AssetManager : ILClass<AssetManager, 0x04c587b8> {
        struct StaticFields {
            int32_t Timeout;
            System::Byte_array* _preallocatedBuffer;
            void* _errorRequestList;
            void* _requestPacket;
            void* _dependencyRequestPacket;
            void* _waitObject;
            AssetBundleDownloadManifest::Object* _defaultManifest;
            bool _cacheEnabled;
            System::Collections::Generic::Dictionary$$String__AssetBundleDownloadManifest _installedTable;
            bool _ready;
            void* _removePackets;
            void* _shaders;
        };
    };
}