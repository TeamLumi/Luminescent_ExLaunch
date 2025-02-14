#pragma once

#include "externals/il2cpp.h"

#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::UI {
    struct WazaManagePokemonStausPanel : ILClass<WazaManagePokemonStausPanel> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            // TODO
        };

        inline void Setup(Pml::PokePara::PokemonParam::Object* param, bool isWazaActive) {
            external<void>(0x01a41520, this, param, isWazaActive);
        }
    };
}
