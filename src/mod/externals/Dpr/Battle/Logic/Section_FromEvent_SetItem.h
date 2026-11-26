#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_SetItem : ILClass<Section_FromEvent_SetItem, game::Address::Dpr_Battle_Logic_Section_FromEvent_SetItem_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_SetItem_Description_TypeInfo> {
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

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_SetItem_Description_ctor, this);
            }
        };
    };
}
