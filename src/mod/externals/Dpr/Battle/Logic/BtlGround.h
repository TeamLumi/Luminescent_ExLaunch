#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class BtlGround : uint8_t {
        BTL_GROUND_NONE = 0,
        BTL_GROUND_GRASS = 1,
        BTL_GROUND_MIST = 2,
        BTL_GROUND_ELEKI = 3,
        BTL_GROUND_PHYCHO = 4,
        BTL_GROUND_MAX = 5,
    };
}