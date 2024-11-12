#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"

namespace UnityEngine {
    struct JsonUtility : ILClass<JsonUtility> {
        static inline void FromJsonOverwrite(System::String::Object* json, Il2CppObject* objectToOverwrite) {
            external<void>(0x02c375d0, objectToOverwrite);
        }

        static inline Il2CppObject* FromJson(System::String::Object* json, System::Type::Object* type) {
            return external<Il2CppObject*>(0x02c37420, json, type);
        }

        static inline System::String::Object* ToJson(Il2CppObject* obj, bool prettyPrint) {
            return external<System::String::Object*>(0x02c372c0, obj, prettyPrint);
        }
    };
}
