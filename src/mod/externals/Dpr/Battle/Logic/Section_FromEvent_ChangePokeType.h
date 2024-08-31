#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeTypePair.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_ChangePokeType : ILClass<Section_FromEvent_ChangePokeType, 0x04c5b8c0> {
        struct Description : ILClass<Description, 0x04c5b2c8> {
            struct Fields {
                uint8_t pokeID;
                PokeTypePair::Object nextType;
                BTL_POKEPARAM::ExTypeCause exTypeCause;
                bool isStandardMessageDisable;
                bool isFailMessageEnable;
                bool isDisplayTokuseiWindow;
                StrParam::Object* changedMessage;
            };

            inline void ctor() {
                external<void>(0x01fb16b0, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5adf8> {
            struct Fields {
                bool isSuccessed;
            };

            inline void ctor() {
                external<void>(0x01fb1750, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
