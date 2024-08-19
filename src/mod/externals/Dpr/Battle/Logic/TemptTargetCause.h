#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class TemptTargetCause : uint8_t {
        NONE = 0,
        YOBIMIZU = 1,
        HIRAISIN = 2,
        OTHER = 3,
    };
}