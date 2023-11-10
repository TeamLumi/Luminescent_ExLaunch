#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct PokeTypePair : ILStruct<PokeTypePair> {
        struct Fields {
            uint16_t value;
        };
    };
}
