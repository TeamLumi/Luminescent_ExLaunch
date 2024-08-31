#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_SetWazaEffectEnable : ILClass<Section_FromEvent_SetWazaEffectEnable, 0x04c5ba40> {
        struct Description : ILClass<Description, 0x04c5b368> {
            inline void ctor() {
                external<void>(0x01fb7ed0, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5af78> {
            inline void ctor() {
                external<void>(0x01fb7ee0, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
