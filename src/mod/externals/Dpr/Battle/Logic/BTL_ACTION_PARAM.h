#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct BTL_ACTION_PARAM : ILStruct<BTL_ACTION_PARAM> {
        struct Fields {
            int64_t raw;
        };
    };
}