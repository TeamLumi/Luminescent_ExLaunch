#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FieldEffect_Add : ILClass<Section_FieldEffect_Add, 0x04c5b930> {
        struct Description : ILClass<Description, 0x04c5b2f8> {
            struct Fields {
                uint8_t pokeID;
                int32_t effect;
                uint8_t ground;
                uint8_t ground_cause;
                BTL_SICKCONT::Object cont;
                uint16_t successEffectNo;
                StrParam::Object* successMessage;
                bool isAddDependPoke;
                bool isDisplayTokuseiWindow;
            };

            inline void ctor() {
                external<void>(0x021b8240, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5ae60> {
            struct Fields {
                bool isSuccessed;
            };

            inline void ctor() {
                external<void>(0x021b82e0, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
