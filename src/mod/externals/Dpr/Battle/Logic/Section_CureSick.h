#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"
#include "externals/Dpr/Battle/Logic/WazaSickEx.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct Section_CureSick : ILClass<Section_CureSick, 0x04c5b8f0> {
        struct Description : ILClass<Description, 0x04c5b1a8> {
            struct Fields {
                uint8_t pokeID;
                WazaSickEx sick;
                System::Byte_array* targetPokeID;
                uint8_t targetPokeCount;
                uint16_t itemID;
                bool isDisplayTokuseiWindow;
                bool isStandardMessageDisable;
                StrParam::Object* successSpMessage;
            };

            inline void ctor() {
                external<void>(0x021b48b0, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
