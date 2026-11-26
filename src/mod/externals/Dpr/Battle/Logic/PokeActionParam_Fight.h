#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct PokeActionParam_Fight : ILClass<PokeActionParam_Fight, game::Address::Dpr_Battle_Logic_PokeActionParam_Fight_TypeInfo> {
        struct Fields {
            uint8_t targetPos;
            uint8_t aimTargetID;
            int32_t waza;
            bool gFlag;
            bool forbidGWaza;
            bool forceGWaza;
        };
    };
}
