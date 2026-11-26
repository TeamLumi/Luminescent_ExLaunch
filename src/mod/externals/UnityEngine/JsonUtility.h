#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"

namespace UnityEngine {
    struct JsonUtility : ILClass<JsonUtility> {
        static inline void FromJsonOverwrite(System::String::Object* json, Il2CppObject* objectToOverwrite) {
            external<void>(game::Address::UnityEngine_JsonUtility_FromJsonOverwrite, objectToOverwrite);
        }

        static inline Il2CppObject* FromJson(System::String::Object* json, System::Type::Object* type) {
            return external<Il2CppObject*>(game::Address::UnityEngine_JsonUtility_FromJson, json, type);
        }

        static inline System::String::Object* ToJson(Il2CppObject* obj, bool prettyPrint) {
            return external<System::String::Object*>(game::Address::UnityEngine_JsonUtility_ToJson, obj, prettyPrint);
        }
    };
}
