#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/SerializedPokemonFull.h"

namespace Dpr::Box {
    struct SaveBoxTrayData : ILStruct<SaveBoxTrayData, 0x04c64da0> {
        struct Fields {
            Pml::PokePara::SerializedPokemonFull::Array* pokemonParam;
        };

        static long GetByteCount() {
            return Pml::PokePara::SerializedPokemonFull::GetByteCount()*30;
        }
    };
}