#pragma once

#include "externals/il2cpp-api.h"

namespace System {
    struct Math : ILClass<Math, game::Address::System_Math_TypeInfo> {
        static inline uint8_t Min(uint8_t val1, uint8_t val2) {
            return external<uint8_t>(game::Address::System_Math_Min, val1, val2);
        }

        static inline int32_t Max(int32_t val1, int32_t val2) {
            return external<int32_t>(game::Address::System_Math_Max_Int32_Int32, val1, val2);
        }
    };
}