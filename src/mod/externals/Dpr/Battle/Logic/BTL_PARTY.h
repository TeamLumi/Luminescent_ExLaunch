#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"

namespace Dpr::Battle::Logic {
    struct BTL_PARTY : ILClass<BTL_PARTY> {
        struct Fields {
            Dpr::Battle::Logic::BTL_POKEPARAM::Array* m_pMember;
            uint8_t m_memberCount;
        };

        inline BTL_POKEPARAM::Object* GetMemberDataConst(uint8_t idx) {
            return external<BTL_POKEPARAM::Object*>(0x01fd9c40, this, idx);
        }
    };
}
