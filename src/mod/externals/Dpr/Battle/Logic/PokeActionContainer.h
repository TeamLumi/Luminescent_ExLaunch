#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeAction.h"

namespace Dpr::Battle::Logic {
    struct PokeActionContainer : ILClass<PokeActionContainer, game::Address::Dpr_Battle_Logic_PokeActionContainer_TypeInfo> {
        struct Fields {
            PokeAction::Array* m_actions;
            uint8_t m_count;
        };

        inline void Add(Dpr::Battle::Logic::PokeAction::Object** action) {
            external<void>(game::Address::Dpr_Battle_Logic_PokeActionContainer_Add, this, action);
        }
    };
}
