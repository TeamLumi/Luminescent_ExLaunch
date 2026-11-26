#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct PokeActionParam_Item : ILClass<PokeActionParam_Item, game::Address::Dpr_Battle_Logic_PokeActionParam_Item_TypeInfo> {
        struct Fields {
            uint8_t targetID;
            uint16_t number;
            uint8_t param;
        };
    };
}
