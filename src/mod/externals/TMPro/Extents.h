#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Vector2.h"

namespace TMPro {
    struct Extents : ILStruct<Extents> {
        struct Fields {
            UnityEngine::Vector2::Object min;
            UnityEngine::Vector2::Object max;
        };

        static_assert(offsetof(Fields, max) == 0x8);
    };
}
