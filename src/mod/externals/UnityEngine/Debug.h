#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct Debug : ILClass<Debug, 0x04c571d8> {
        struct Fields {};

        inline static void Log(Il2CppObject* message) {
            external<void>(0x026a9c10, message);
        }

        inline static void Log(System::String::Object* message) {
            Log((Il2CppObject*)message);
        }
    };
}

