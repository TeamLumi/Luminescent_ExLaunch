#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_ProcessActionCore : ILClass<Section_ProcessActionCore, game::Address::Dpr_Battle_Logic_Section_ProcessActionCore_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_ProcessActionCore_Description_TypeInfo> {
            struct Fields {
                PokeAction::Object* pokeAction;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_ProcessActionCore_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_ProcessActionCore_Result_TypeInfo> {
            struct Fields {};

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_ProcessActionCore_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};

        inline void action_Cheer(PokeAction::Object* pPokeAction) {
            external<void>(game::Address::Dpr_Battle_Logic_Section_ProcessActionCore_action_Cheer, this, pPokeAction);
        }

        inline void reinforceRaidBoss(PokeAction::Object* pokeAction) {
            external<void>(game::Address::Dpr_Battle_Logic_Section_ProcessActionCore_reinforceRaidBoss, this, pokeAction);
        }

        inline void action(PokeAction::Object* pokeAction) {
            external<void>(game::Address::Dpr_Battle_Logic_Section_ProcessActionCore_action, this, pokeAction);
        }
    };
}
