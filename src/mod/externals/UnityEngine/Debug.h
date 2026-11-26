#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct Debug : ILClass<Debug, game::Address::UnityEngine_Debug_TypeInfo> {
        struct Fields {};

        inline static void Log(Il2CppObject* message) {
            external<void>(game::Address::UnityEngine_Debug_Log_Object, message);
        }

        inline static void Log(System::String::Object* message) {
            Log((Il2CppObject*)message);
        }
    };
}

