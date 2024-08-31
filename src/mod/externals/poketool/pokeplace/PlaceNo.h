#pragma once

#include "externals/il2cpp-api.h"

namespace poketool::pokeplace {
    enum PlaceNo : int32_t {
        START_NORMAL = 0,
        START_SPECIAL = 30001,
        START_OUTER = 40001,
        START_PERSON = 60001,
        END = 65535,
        PERSON_SODATEYA = 60010,
        SPECIAL_INNER_TRADE = 30001,
        SPECIAL_OUTER_TRADE = 30002,
    };
}
