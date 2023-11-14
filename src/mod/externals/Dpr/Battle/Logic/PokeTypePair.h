#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct PokeTypePair : ILStruct<PokeTypePair> {
        struct Fields {
            uint16_t value;
        };

        static inline PokeTypePair::Object MakePure(uint8_t type) {
            return external<PokeTypePair::Object>(0x020ce5d0, type);
        }
    };
}
