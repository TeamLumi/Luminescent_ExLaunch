#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class RankEffectCause : uint8_t {
        OTHER = 0,
        IKAKU = 1,
        NUMENUME = 2,
        KINGSHIELD = 3,
        NEBANEBANET = 4,
        RAID_BOSS_EXTRA_ATTACK = 5,
        RAID_BOSS_BREAK_GWALL = 6,
        GSHOCK = 7,
        CHEER = 8,
        MIRRORARMOR = 9,
        TAKOGATAME = 10,
    };
}