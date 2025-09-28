#pragma once

#include <cstdint>

namespace Pml::PokePara {
    struct SavePokeParty : ILStruct<SavePokeParty> {
        struct Fields {
            Pml::PokePara::SerializedPokemonFull::Array* members;
            uint8_t memberCount;
            uint8_t markingIndex;
        };
    };
}

