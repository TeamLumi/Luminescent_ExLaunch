#pragma once

#include "externals/il2cpp-api.h"

namespace System {
    struct Math : ILClass<Math, 0x04c55468> {
        static inline uint8_t Min(uint8_t val1, uint8_t val2) {
            return external<uint8_t>(0x022ac480, val1, val2);
        }
    };
}