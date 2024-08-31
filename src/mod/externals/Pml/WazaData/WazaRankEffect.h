#pragma once

#include "externals/il2cpp-api.h"

namespace Pml::WazaData {
    enum class WazaRankEffect : int32_t {
        NONE = 0,
        ORIGIN = 1,
        ATTACK = 1,
        DEFENCE = 2,
        SP_ATTACK = 3,
        SP_DEFENCE = 4,
        AGILITY = 5,
        HIT = 6,
        AVOID = 7,
        MAX = 8,
        BASE_MAX = 6,
        NUMS = 7,
        SP = 8,
        CRITICAL_RATIO = 9,
        MULTI5 = 10,
    };
}