#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace UnityEngine {
    struct PlayerPrefs : ILClass<PlayerPrefs> {
        static inline void SetString(System::String::Object* key, System::String::Object* value) {
            external<void>(0x023250f8, key, value);
        }

        static inline System::String::Object* GetString(System::String::Object* key) {
            return external<System::String::Object*>(0x026915b0, key);
        }
    };
}
