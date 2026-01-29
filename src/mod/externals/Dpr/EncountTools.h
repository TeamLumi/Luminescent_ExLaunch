#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Pml/PokePara/PokemonParam.h"

namespace Dpr {
    struct EncountTools : ILClass<EncountTools, 0x4c59e90> {

        static inline uint16_t SetWildPokemonItem(Pml::PokePara::PokemonParam* pp, int32_t itemRnd, int32_t rnd1 , int32_t rnd2) {
            return external<uint16_t>(0x02c3aab0, pp, itemRnd, rnd1, rnd2);
        }

    };
}


