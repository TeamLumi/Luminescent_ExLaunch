#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"
#include "externals/Dpr/Battle/Logic/Section_CommonParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_SwapItem : ILClass<Section_FromEvent_SwapItem, game::Address::Dpr_Battle_Logic_Section_FromEvent_SwapItem_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_SwapItem_Description_TypeInfo> {
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
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_SwapItem_Description_ctor, this);
            }
        };
    };
}
