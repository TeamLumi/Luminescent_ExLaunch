#pragma once

#include "externals/il2cpp-api.h"

namespace Pml::PokePara {
    enum class RareType : uint8_t {
        NOT_RARE = 0,
        CAPTURED = 1,
        DISTRIBUTED = 2,
        NUM = 3,
    };
}