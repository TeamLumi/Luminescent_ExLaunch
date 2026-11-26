#pragma once

#include <cstdint>

namespace exl::util {
    enum class GameVersion : uint8_t {
        Unknown,
        BrilliantDiamond130,
        ShiningPearl130
    };

    namespace impl {
        GameVersion DetermineGameVersion();
    }
}
