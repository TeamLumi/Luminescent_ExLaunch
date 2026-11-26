#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_InterruptAction : ILClass<Section_InterruptAction, game::Address::Dpr_Battle_Logic_Section_InterruptAction_TypeInfo> {
        struct Description : ILClass<Description> {
            struct Fields {
                uint8_t interruptPokeID;
                uint8_t targetPokeID;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_InterruptAction_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result> {
            struct Fields {
                bool isInterrupted;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_InterruptAction_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};

        inline void processAction(PokeAction::Object* pokeAction) {
            external<void>(game::Address::Dpr_Battle_Logic_Section_InterruptAction_processAction, this, pokeAction);
        }
    };
}
