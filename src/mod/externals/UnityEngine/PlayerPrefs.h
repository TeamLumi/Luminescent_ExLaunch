#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace UnityEngine {
    struct PlayerPrefs : ILClass<PlayerPrefs> {
        static inline void SetString(System::String::Object* key, System::String::Object* value) {
            external<void>(game::Address::SmartPoint_Components_PlayerPrefsProvider_PlayerWork_Save + 0x198, key, value);
        }

        static inline System::String::Object* GetString(System::String::Object* key) {
            return external<System::String::Object*>(game::Address::UnityEngine_PlayerPrefs_GetString, key);
        }
    };
}
