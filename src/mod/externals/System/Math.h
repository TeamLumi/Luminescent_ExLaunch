#pragma once

#include "externals/il2cpp-api.h"

namespace System {
    struct Math : ILClass<Math, 0x04c55468> {
        static inline uint8_t Min(uint8_t val1, uint8_t val2) {
            return external<uint8_t>(0x022ac480, val1, val2);
        }

        static inline int32_t Max(int32_t val1, int32_t val2) {
            return external<int32_t>(0x022ac350, val1, val2);
        }
    };
}