#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_SetItem : ILClass<Section_FromEvent_SetItem> {
        struct Description : ILClass<Description, 0x04c5b1f8> {
            struct Fields {
                uint8_t userPokeID;
                uint8_t targetPokeID;
                uint16_t itemID;
                bool isClearConsume;
                bool isClearConsumeOtherPoke;
                uint8_t clearConsumePokeID;
                bool isCallConsumedEvent;
                bool isDisplayTokuseiWindow;
                bool isConsumeItem;
                StrParam::Object* successMessage;
            };

            void ctor() {
                return external<void>(0x01fb7ab0, this);
            }
        };
    };
}
