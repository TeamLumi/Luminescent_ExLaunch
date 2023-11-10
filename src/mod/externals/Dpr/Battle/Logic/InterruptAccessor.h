#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct InterruptAccessor : ILClass<InterruptAccessor> {
        struct Fields {
            uint8_t m_interrupt;
        };

        inline bool IsRequested(InterruptCode _interrupt) {
            return external<bool>(0x0202c020, this, _interrupt);
        }
    };
}
