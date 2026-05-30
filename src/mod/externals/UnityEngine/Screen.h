#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct Screen : ILClass<Screen> {
        struct Fields {};

        inline static void SetResolution(int32_t width, int32_t height) {
            typedef void(*methodPtr)(int32_t, int32_t);
            auto ptr = (methodPtr)resolve_unity_method("UnityEngine.Screen::SetResolution(System.Int32,System.Int32)");
            ptr(width, height);
        }
    };
}
