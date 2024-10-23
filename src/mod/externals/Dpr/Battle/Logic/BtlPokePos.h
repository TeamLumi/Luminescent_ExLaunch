#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class BtlPokePos : uint8_t {
        POS_1ST_0 = 0,
        POS_2ND_0 = 1,
        POS_1ST_1 = 2,
        POS_2ND_1 = 3,
        POS_RAID_PLAYER_0 = 0,
        POS_RAID_PLAYER_1 = 1,
        POS_RAID_PLAYER_2 = 2,
        POS_RAID_PLAYER_3 = 3,
        POS_RAID_BOSS = 4,
        POS_NULL = 5,
    };

    PRIMITIVE_ARRAY(BtlPokePos, 0x04c5a7f8)
}
