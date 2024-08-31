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

        static inline void Split(PokeTypePair::Object pair, uint8_t* type1, uint8_t* type2, uint8_t* typeEx) {
            external<void>(0x020ce620, pair, type1, type2, typeEx);
        }
    };
}
