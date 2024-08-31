#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_FieldEffect_End : ILClass<Section_FieldEffect_End, 0x04c5b7c8> {
        struct Description : ILClass<Description, 0x04c5bf28> {
            struct Fields {
                int32_t effect;
                BTL_POKEPARAM::Object* pDependPoke;
            };

            inline void ctor() {
                external<void>(0x021b53a0, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5bf30> {
            struct Fields {
                bool isRemoved;
            };

            inline void ctor() {
                external<void>(0x021b53e0, this);
            }
        };

        struct Fields : Section::Fields {};

        inline void checkItemReaction_All() {
            external<void>(0x021b8460, this);
        }

        inline void resetGround() {
            external<void>(0x021b8540, this);
        }

        inline void resetKagakuhenkaGas() {
            external<void>(0x021b8680, this);
        }
    };
}
