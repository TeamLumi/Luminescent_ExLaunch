#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Dpr/Battle/Logic/BtlSideEffect.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_SideEffect_Add : ILClass<Section_SideEffect_Add, 0x04c5ba68> {
        struct Description : ILClass<Description, 0x04c5b3c8> {
            struct Fields {
                uint8_t pokeID;
                BtlSideEffect effect;
                BTL_SICKCONT::Object cont;
                int32_t side;
                uint16_t successEffectNo;
                StrParam::Object* successMessage;
                bool isReplaceSuccessMessageArgs0ByExpandSide;
            };

            inline void ctor() {
                external<void>(0x021cb080, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5ae38> {
            struct Fields {
                bool isSuccessed;
            };

            inline void ctor() {
                external<void>(0x021cb120, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
