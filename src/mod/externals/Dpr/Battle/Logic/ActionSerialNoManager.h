#pragma once
#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct ActionSerialNoManager : ILClass<ActionSerialNoManager> {
        struct Fields {
            uint32_t m_serialNo;
        };

        inline void AssignSerialNo(void* actionDesc) {
            external<void>(game::Address::Dpr_Battle_Logic_ActionSerialNoManager_AssignSerialNo, this, actionDesc);
        };
    };
}
