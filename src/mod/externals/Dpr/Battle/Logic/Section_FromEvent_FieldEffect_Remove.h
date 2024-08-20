#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_FieldEffect_Remove : ILClass<Section_FromEvent_FieldEffect_Remove, 0x04c5b938> {
        struct Description : ILClass<Description, 0x04c5b300> {
            struct Fields {
                int32_t effect;
                BTL_POKEPARAM::Object* pDependPoke;
            };

            inline void ctor() {
                external<void>(0x01fb4390, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5ae68> {
            struct Fields {
                bool isSuccessed;
            };

            inline void ctor() {
                external<void>(0x01fb43d0, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
