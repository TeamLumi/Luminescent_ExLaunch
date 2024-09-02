#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/System/Type.h"

namespace UnityEngine {
    struct JsonUtility : ILClass<JsonUtility> {
        struct Fields {};

        inline static void* FromJson(System::String::Object* json, System::Type::Object* type) {
            return external<void*>(0x02c37420, json, type);
        }
    };
}