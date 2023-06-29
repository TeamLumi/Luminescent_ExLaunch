#pragma once

#include "externals/il2cpp.h"

#include "externals/System/String.h"
#include "externals/UnityEngine/_Object.h"

namespace UnityEngine {
    struct AssetBundle : ILClass<AssetBundle> {
        struct Fields : UnityEngine::_Object::Fields {

        };

        inline UnityEngine::_Object::Object* LoadAsset(System::String::Object* name) {
            return external<UnityEngine::_Object::Object*>(0x02c317c0, this, name);
        }

        static inline UnityEngine::AssetBundle::Object* LoadFromFile(System::String::Object* path) {
            return external<UnityEngine::AssetBundle::Object*>(0x02c316d0, path);
        }
    };
}
