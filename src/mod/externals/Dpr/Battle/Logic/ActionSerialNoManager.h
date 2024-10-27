#pragma once
#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct ActionSerialNoManager : ILClass<ActionSerialNoManager> {
        struct Fields {
            uint32_t m_serialNo;
        };

        inline void AssignSerialNo(void* actionDesc) {
            external<void>(0x01abdd30, this, actionDesc);
        };
    };
}
