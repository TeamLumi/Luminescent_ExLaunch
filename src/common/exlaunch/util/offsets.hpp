#pragma once

#include <cstdint>
#include <program/offsets.hpp>
#include <program/typeinfo.hpp>
#include "version.hpp"

namespace exl::util::offsets {

    using Game = GameVersion;

    inline Game GetGame() { return GetGameVersion(); }

    inline uintptr_t Resolve(game::Location address) {
        const auto& entry = game::Addresses[static_cast<size_t>(address.symbol)];
        if (GetGame() == Game::ShiningPearl130)
            return entry.sp + address.offset.sp;
        return entry.bd + address.offset.bd;
    }
}
