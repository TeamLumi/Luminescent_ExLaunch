#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/UnityEngine/Transform.h"

namespace Dpr::FureaiHiroba {
    struct PokeFactory : ILClass<PokeFactory> {
        static inline void SetPokeScale(UnityEngine::Transform::Object* transform, Pml::PokePara::PokemonParam::Object* pokemonParam) {
            external<void>(0x1A7EA40, transform, pokemonParam);
        }
    };
}
