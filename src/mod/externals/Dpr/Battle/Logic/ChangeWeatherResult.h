#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class ChangeWeatherResult : uint8_t {
        OK = 0,
        FAIL = 1,
        FAIL_CANT_OVERWRITE = 2,
    };
}
