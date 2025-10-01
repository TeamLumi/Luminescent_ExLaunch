#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/ScriptableObject.h"

namespace TMPro {
    struct TMP_ColorGradient : ILClass<TMP_ColorGradient> {
        struct Fields : UnityEngine::ScriptableObject::Fields {
            int32_t colorMode;
            UnityEngine::Color::Object topLeft;
            UnityEngine::Color::Object topRight;
            UnityEngine::Color::Object bottomLeft;
            UnityEngine::Color::Object bottomRight;
        };
    };
}
