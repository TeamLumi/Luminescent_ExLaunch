#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class TemptTargetPriority : uint8_t {
        NONE = 0,
        TOKUSEI = 1,
        WAZA_1 = 2,
        WAZA_2 = 3,
    };
}