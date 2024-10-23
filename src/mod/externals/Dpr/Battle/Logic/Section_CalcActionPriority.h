#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/PokeAction.h"

namespace Dpr::Battle::Logic {
    struct Section_CalcActionPriority : ILClass<Section_CalcActionPriority> {
        struct Description : ILClass<Description> {
            struct Fields {
                PokeAction::Object* pokeAction;
                uint8_t dominantPriority;
                int32_t specialPriority;
            };
        };

        struct Result : ILClass<Result> {
            struct Fields {
                uint32_t priority;
            };
        };

        struct Fields : Dpr::Battle::Logic::Section::Fields {

        };
    };
}
