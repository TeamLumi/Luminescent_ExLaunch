#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class PokeActionCategory : int32_t {
        Null = 0,
        Fight = 1,
        Item = 2,
        PokeChange = 3,
        Escape = 4,
        Skip = 5,
        GStart = 6,
        Cheer = 7,
        RaidBossExtraAction = 8,
        RecPlay_TimeOver = 9,
        RecPlay_Error = 10,
    };
}
