#pragma once

#include "externals/il2cpp-api.h"

namespace TMPro {
    struct TMP_FontStyleStack : ILStruct<TMP_FontStyleStack> {
        struct Fields {
            uint8_t bold;
            uint8_t italic;
            uint8_t underline;
            uint8_t strikethrough;
            uint8_t highlight;
            uint8_t superscript;
            uint8_t subscript;
            uint8_t uppercase;
            uint8_t lowercase;
            uint8_t smallcaps;
        };

        static_assert(offsetof(Fields, smallcaps) == 0x9);
    };
}
