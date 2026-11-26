#include "version.hpp"
#include <common.hpp>
#include <checks.hpp>

namespace exl::util::impl {
    GameVersion DetermineGameVersion() {
        if (is_game_version(DIAMOND, "1.3.0"))
            return GameVersion::BrilliantDiamond130;
        if (is_game_version(PEARL, "1.3.0"))
            return GameVersion::ShiningPearl130;

        EXL_ABORT(0, "Unsupported game version");
    }
}
