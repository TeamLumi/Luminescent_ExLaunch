#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct PokeSet : ILClass<PokeSet> {
        struct Fields {
            BTL_POKEPARAM::Array* m_bpp;
            System::UInt16_array* m_damage;
            System::UInt16_array* m_migawariDamage;
            System::Byte_array* m_damageType;
            System::UInt16_array* m_sortWork;
            uint8_t m_count;
            uint8_t m_countMax;
            uint8_t m_getIdx;
            uint8_t m_targetPosCount;
        };

        inline BTL_POKEPARAM::Object* Get(uint32_t idx) {
            return external<BTL_POKEPARAM::Object*>(0x020cd920, this, idx);
        }
    };
}
