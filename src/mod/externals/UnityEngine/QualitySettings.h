#pragma once
#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct QualitySettings : ILClass<QualitySettings> {
        static inline int32_t get_vSyncCount() {
            return external<int32_t>(0x26920D0);
        }

        // Resolve a Unity internal call by name at runtime.
        // The IL2CPP icall resolver at RVA 0x2AFA60 takes a const char*.
        static inline void* resolve_icall(const char* name) {
            return external<void*>(0x2AFA60, name);
        }

        // set_vSyncCount was not compiled into the IL2CPP binary (game code never calls it),
        // but the native implementation exists in the Unity runtime.
        static inline void set_vSyncCount(int32_t value) {
            typedef void (*set_vSyncCount_t)(int32_t);
            static set_vSyncCount_t fn = nullptr;
            if (fn == nullptr) {
                fn = (set_vSyncCount_t)resolve_icall("UnityEngine.QualitySettings::set_vSyncCount(System.Int32)");
            }
            if (fn != nullptr)
                fn(value);
        }
    };
}
