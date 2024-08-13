#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_FIELD_SITUATION.h"

namespace Dpr::Battle::Logic {
    struct tables : ILClass<tables, 0x04c5a8c0> {
        static inline bool IsSakidoriFailWaza(int32_t waza) {
            return external<bool>(0x01f78540, waza);
        }
    };
}