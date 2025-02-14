#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    // The proper fields are separate r, g, b, and a, but this messes up alignment it seems?
    struct Color32 : ILStruct<Color32> {
        #pragma pack(push, 1)
        struct Fields {
            //int32_t rgba;
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };
        #pragma pack(pop)

        static_assert(offsetof(Fields, a) == 0x3);
        static_assert(sizeof(Fields) == 0x4);
    };
}
