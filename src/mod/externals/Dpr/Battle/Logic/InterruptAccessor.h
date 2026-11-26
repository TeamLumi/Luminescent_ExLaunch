#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct InterruptAccessor : ILClass<InterruptAccessor> {
        struct Fields {
            uint8_t m_interrupt;
        };

        inline bool IsRequested(InterruptCode _interrupt) {
            return external<bool>(game::Address::Dpr_Battle_Logic_InterruptAccessor_IsRequested, this, _interrupt);
        }
    };
}
