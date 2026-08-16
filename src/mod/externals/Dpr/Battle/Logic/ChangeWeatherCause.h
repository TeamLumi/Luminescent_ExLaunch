#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class ChangeWeatherCause : uint8_t {
        OTHERS = 0,
        ON_BATTLE_START = 1,
    };
}
