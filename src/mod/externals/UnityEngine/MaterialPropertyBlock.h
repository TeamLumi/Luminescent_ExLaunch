#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct MaterialPropertyBlock : ILClass<MaterialPropertyBlock> {
        struct Fields {
            intptr_t m_Ptr;
        };

        inline void SetColor(int32_t nameID, UnityEngine::Color::Object value) {
            UnityEngine::Color::Fields valueProxy = { .r = value.fields.r, .g = value.fields.g, .b = value.fields.b, .a = value.fields.a };
            external<void>(0x026b8290, this, nameID, valueProxy);
        }
    };
}