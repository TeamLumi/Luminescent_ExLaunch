#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct PokeID : ILClass<PokeID, game::Address::Dpr_Battle_Logic_PokeID_TypeInfo> {
        static const uint8_t INVALID = 31;

        static inline int32_t PokeIdtoClientId(uint8_t pokeID) {
            return external<int32_t>(game::Address::Dpr_Battle_Logic_PokeID_PokeIdToClientId, pokeID);
        }
    };
}
