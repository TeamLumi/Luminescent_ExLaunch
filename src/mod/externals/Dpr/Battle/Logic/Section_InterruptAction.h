#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_InterruptAction : ILClass<Section_InterruptAction, 0x04c5bae0> {
        struct Description : ILClass<Description> {
            struct Fields {
                uint8_t interruptPokeID;
                uint8_t targetPokeID;
            };

            inline void ctor() {
                external<void>(0x021bd000, this);
            }
        };

        struct Result : ILClass<Result> {
            struct Fields {
                bool isInterrupted;
            };

            inline void ctor() {
                external<void>(0x021bd030, this);
            }
        };

        struct Fields : Section::Fields {};

        inline void processAction(PokeAction::Object* pokeAction) {
            external<void>(0x01fbd6a0, this, pokeAction);
        }
    };
}
