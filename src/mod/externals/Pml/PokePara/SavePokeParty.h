#pragma once

#include <cstdint>
#include "externals/il2cpp-api.h"
#include "externals/Pml/PokePara/SerializedPokemonFull.h"

namespace Pml {
    struct PokeParty;
}

namespace Pml::PokePara {
    struct SavePokeParty : ILStruct<SavePokeParty> {
        struct Fields {
            Pml::PokePara::SerializedPokemonFull::Array* members;
            uint8_t memberCount;
            uint8_t markingIndex;
        };

        inline void Serialize_Full(Pml::PokeParty* party) {
            external<void>(0x02055490, this, party);
        }

        inline void Deserialize_Full(Pml::PokeParty* party) {
            external<void>(0x02055730, this, party);
        }

        inline void CreateWorkIfNeed() {
            external<void>(0x02055610, this);
        }

        inline void Clear() {
            external<void>(0x02055990, this);
        }
    };
}

