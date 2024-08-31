#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct PokeActionParam_PokeChange : ILClass<PokeActionParam_PokeChange, 0x04c5b5b8> {
        struct Fields {
            uint8_t posIdx;
            uint8_t memberIdx;
            bool depleteFlag;
        };
    };
}
