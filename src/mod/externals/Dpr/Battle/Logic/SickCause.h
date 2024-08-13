#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class SickCause : uint8_t {
        OTHER = 0,
        TOKUSEI_TOUCH_COUNTER = 1,
        TOOTIKA = 2,
        HOROBINO_BODY = 3,
        DOKUBISI = 4,
        WAZA_EFFECT_SICK = 5,
    };
}