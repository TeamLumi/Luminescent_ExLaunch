#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class SickOverWriteMode : uint8_t {
        CANT = 0,
        POKESICK = 1,
        STRONG = 2,
    };
}