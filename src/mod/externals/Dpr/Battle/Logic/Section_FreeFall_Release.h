#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/WazaParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FreeFall_Release : ILClass<Section_FreeFall_Release, 0x04c5b888> {
        struct Description : ILClass<Description, 0x04c5bf20> {
            struct Fields {
                BTL_POKEPARAM::Object* attacker;
                bool canAppearSelf;
                bool canAppearTarget;
            };

            inline void ctor() {
                external<void>(0x01fb0c70, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5af98> {
            struct Fields {};

            inline void ctor() {
                external<void>(0x01fb0cb0, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
