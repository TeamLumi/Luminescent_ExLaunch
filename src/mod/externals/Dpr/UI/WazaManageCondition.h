#pragma once

#include "externals/il2cpp.h"

#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::UI {
    struct WazaManageCondition : ILClass<WazaManageCondition> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            // TODO
        };

        inline void Setup(Pml::PokePara::PokemonParam::Object* pokemonParam) {
            external<void>(0x01a41350, this, pokemonParam);
        }
    };
}
