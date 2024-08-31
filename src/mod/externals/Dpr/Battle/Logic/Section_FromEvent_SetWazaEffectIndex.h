#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"
#include "externals/Dpr/Battle/Logic/CommonParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_SetWazaEffectIndex : ILClass<Section_FromEvent_SetWazaEffectIndex, 0x04c5ba48> {
        struct Description : ILClass<Description, 0x04c5b350> {
            struct Fields {
                uint8_t effectIndex;
            };

            inline void ctor() {
                external<void>(0x01fb7f40, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5af70> {
            inline void ctor() {
                external<void>(0x01fb7f70, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
