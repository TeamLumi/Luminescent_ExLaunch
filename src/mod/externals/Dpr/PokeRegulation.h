#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Pml/PokeParty.h"

namespace Dpr {
    struct PokeRegulation : ILClass<PokeRegulation, 0x04c5ad08> {
        static inline void ModifyLevelPokeParty(Pml::PokeParty::Object* iPtrPokeParty,
                                                uint8_t levelRangeType, uint32_t modify_level) {
            external<void>(0x01bcd5e0, iPtrPokeParty, levelRangeType, modify_level);
        }

        static inline void ModifyLevelPokeParam(Pml::PokePara::PokemonParam::Object* pp, uint8_t levelRangeType,
                                                uint32_t modify_level) {
            external<void>(0x01bcd6b0, pp, levelRangeType, modify_level);
        }
    };
}
