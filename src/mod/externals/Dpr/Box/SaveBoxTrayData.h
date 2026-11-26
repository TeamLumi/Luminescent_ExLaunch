#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/SerializedPokemonFull.h"

namespace Dpr::Box {
    struct SaveBoxTrayData : ILStruct<SaveBoxTrayData, game::Address::Dpr_Box_SaveBoxTrayData_Array_TypeInfo> {
        struct Fields {
            Pml::PokePara::SerializedPokemonFull::Array* pokemonParam;
        };

        static long GetByteCount() {
            return Pml::PokePara::SerializedPokemonFull::GetByteCount()*30;
        }
    };
}