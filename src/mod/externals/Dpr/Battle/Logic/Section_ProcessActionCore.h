#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_ProcessActionCore : ILClass<Section_ProcessActionCore, 0x04c5bb70> {
        struct Description : ILClass<Description, 0x04c5c358> {
            struct Fields {
                PokeAction::Object* pokeAction;
            };

            inline void ctor() {
                external<void>(0x021c0430, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5c350> {
            struct Fields {};

            inline void ctor() {
                external<void>(0x021c0420, this);
            }
        };

        struct Fields : Section::Fields {};

        inline void action_Cheer(PokeAction::Object* pPokeAction) {
            external<void>(0x021c0b20, this, pPokeAction);
        }

        inline void reinforceRaidBoss(PokeAction::Object* pokeAction) {
            external<void>(0x021c0cb0, this, pokeAction);
        }

        inline void action(PokeAction::Object* pokeAction) {
            external<void>(0x021c0de0, this, pokeAction);
        }
    };
}
