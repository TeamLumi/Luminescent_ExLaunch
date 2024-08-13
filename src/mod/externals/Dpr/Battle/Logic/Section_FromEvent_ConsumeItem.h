#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeTypePair.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_ConsumeItem : ILClass<Section_FromEvent_ConsumeItem, 0x04c5b8e8> {
        struct Description : ILClass<Description, 0x04c5b1d0> {
            struct Fields {
                uint8_t userPokeID;
                bool isUseActionDisable;
                bool isKinomiCheckDisable;
                bool isConsumeMessageEnable;
                StrParam::Object* successMessage;
            };

            inline void ctor() {
                external<void>(0x01fb2dc0, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5aec8> {
            struct Fields {};

            inline void ctor() {
                external<void>(0x01fb2e40, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
