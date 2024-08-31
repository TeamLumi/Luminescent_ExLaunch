#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct PokeActionParam_Item : ILClass<PokeActionParam_Item, 0x04c5b5b0> {
        struct Fields {
            uint8_t targetID;
            uint16_t number;
            uint8_t param;
        };
    };
}
