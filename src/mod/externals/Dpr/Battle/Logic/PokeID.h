#pragma once

#include "externals/il2cpp-api.h"

struct PokeID : ILClass<PokeID, 0x04c5a678> {

    static inline int32_t PokeIdtoClientId(uint8_t pokeID) {
        return external<int32_t>(0x020cb6f0, pokeID);
    }

};