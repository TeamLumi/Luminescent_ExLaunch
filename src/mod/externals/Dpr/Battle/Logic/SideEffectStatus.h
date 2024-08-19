#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Dpr/Battle/Logic/BtlSideEffect.h"

namespace Dpr::Battle::Logic {
    struct SideEffectStatus : ILClass<SideEffectStatus> {
        struct DATA : ILClass<DATA> {
            struct Fields {
                BtlSideEffect sideEffect;
                BTL_SICKCONT::Object contParam;
                uint32_t turn_counter;
                uint32_t add_counter;
            };

            static_assert(offsetof(Fields, add_counter) == 0x14);
        };

        struct Fields {
            DATA m_data;
        };

        inline BTL_SICKCONT::Object GetContParam() {
            return external<BTL_SICKCONT::Object>(0x01f71900, this);
        }
    };
}
