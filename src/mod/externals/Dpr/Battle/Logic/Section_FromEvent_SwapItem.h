#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"
#include "externals/Dpr/Battle/Logic/Section_CommonParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_SwapItem : ILClass<Section_FromEvent_SwapItem, 0x04c5ba88> {
        struct Description : ILClass<Description, 0x04c5b1f0> {
            struct Fields {
                uint8_t userPokeID;
                uint8_t targetPokeID;
                bool isIncRecordCount_StealItemFromWildPoke;
                bool isDisplayTokuseiWindow;
                StrParam::Object* successMessage1;
                StrParam::Object* successMessage2;
                StrParam::Object* successMessage3;
            };

            inline void ctor() {
                external<void>(0x01fb8800, this);
            }
        };
    };
}
