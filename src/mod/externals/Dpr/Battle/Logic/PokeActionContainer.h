#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeAction.h"

namespace Dpr::Battle::Logic {
    struct PokeActionContainer : ILClass<PokeActionContainer, 0x04c5ab08> {
        struct Fields {
            PokeAction::Array* m_actions;
            uint8_t m_count;
        };

        inline void Add(Dpr::Battle::Logic::PokeAction::Object** action) {
            external<void>(0x02042920, this, action);
        }
    };
}
