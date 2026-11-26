#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct PokeActionParam_PokeChange : ILClass<PokeActionParam_PokeChange, game::Address::Dpr_Battle_Logic_PokeActionParam_PokeChange_TypeInfo> {
        struct Fields {
            uint8_t posIdx;
            uint8_t memberIdx;
            bool depleteFlag;
        };
    };
}
