#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class EffectType : int32_t {
        EFF_WEATHER = 0,
        EFF_TRICKROOM = 1,
        EFF_JURYOKU = 2,
        EFF_FUIN = 3,
        EFF_WONDERROOM = 4,
        EFF_MAGICROOM = 5,
        EFF_PLASMASHOWER = 6,
        EFF_FAIRY_LOCK = 7,
        EFF_GROUND = 8,
        EFF_KAGAKUHENKAGAS = 9,
        EFF_MAX = 10,
        EFF_NULL = 10,
    };
}