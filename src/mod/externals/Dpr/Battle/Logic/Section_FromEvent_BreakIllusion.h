#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_BreakIllusion : ILClass<Section_FromEvent_BreakIllusion> {
        struct Description : ILClass<Description> {
            struct Fields {
                uint8_t targetPokeID;
                uint8_t _padding[7];
                StrParam::Object* successMessage;
            };
        };

        struct Result : ILClass<Result> {
            struct Fields {
                bool isSucceeded;
            };
        };

        struct Fields : Section::Fields {};
    };
}
