#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"

namespace Dpr::MsgWindow {
    struct MsgWindowManager : ILClass<MsgWindowManager> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            // TODO
        };

        static inline MsgWindowManager::Object* get_Instance() {
            // Dereference MethodInfo first — if the metadata pointer is null
            // (singleton not yet registered), bail before calling get_Instance
            // which would crash dereferencing null MethodInfo->klass.
            auto& method = SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$MsgWindowManager$$get_Instance;
            MethodInfo* mi = *method;
            if (mi == nullptr) return nullptr;
            return _ILExternal::external<MsgWindowManager::Object*>(0x02322450, mi);
        }

        // MsgWindow component at instance offset 0x38
        inline void* get_MsgWindow() {
            return *(void**)((uintptr_t)this + 0x38);
        }

        static inline bool get_IsOpenWindow() {
            return external<bool>(0x01dd9810);
        }

        static inline bool get_IsOpen() {
            return external<bool>(0x01dd9910);
        }
    };
}
