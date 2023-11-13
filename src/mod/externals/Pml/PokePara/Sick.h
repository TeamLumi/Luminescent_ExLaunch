#pragma once

#include "externals/il2cpp-api.h"

namespace Pml::PokePara {
    enum class Sick : uint32_t {
        NONE = 0,
        MAHI = 1, // Paralysis
        NEMURI = 2, // Sleep
        KOORI = 3, // Freeze
        YAKEDO = 4, // Burn
        DOKU = 5, // Poison
        MAX = 6,
    };
}