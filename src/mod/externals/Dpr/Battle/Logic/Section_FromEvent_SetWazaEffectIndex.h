#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"
#include "externals/Dpr/Battle/Logic/CommonParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_SetWazaEffectIndex : ILClass<Section_FromEvent_SetWazaEffectIndex> {
        struct Description : ILClass<Description, 0x04c5b350> {
            struct Fields {
                uint8_t effectIndex;
            };

            void ctor() {
                return external<void>(0x01fb7f40, this);
            }
        };
    };
}
