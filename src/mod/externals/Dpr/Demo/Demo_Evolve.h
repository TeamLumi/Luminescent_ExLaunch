#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Demo/DemoBase.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/Primitives.h"

namespace Dpr::Demo {
    struct Demo_Evolve : ILClass<Demo_Evolve, 0x04c5e600> {
        struct Param : ILClass<Param> {
            struct Fields {
                Pml::PokePara::PokemonParam::Object* pokemonParam;
                int32_t nextMonsNo;
                bool useCancel;
                uint32_t evoRoute;
            };

            static_assert(offsetof(Fields, useCancel) == 0xc);
        };

        struct Fields : Dpr::Demo::DemoBase::Fields {
            // TODO
        };
    };
}
