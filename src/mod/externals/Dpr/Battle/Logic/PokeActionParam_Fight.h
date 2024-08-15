#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct PokeActionParam_Fight : ILClass<PokeActionParam_Fight, 0x04c5b5a8> {
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
