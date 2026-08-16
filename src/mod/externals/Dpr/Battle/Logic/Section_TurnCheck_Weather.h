#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_TurnCheck_Weather : ILClass<Section_TurnCheck_Weather, 0x04c5bcc0> {
        struct Description : ILClass<Description, 0x04c5c528> {
            struct Fields {};

            inline void ctor() {
                external<void>(0x02170330, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5c530> {
            struct Fields {
                bool isExpGet;
            };

            inline void ctor() {
                external<void>(0x02170340, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
