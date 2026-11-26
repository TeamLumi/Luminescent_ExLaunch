#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeTypePair.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_ConsumeItem : ILClass<Section_FromEvent_ConsumeItem, game::Address::Dpr_Battle_Logic_Section_FromEvent_ConsumeItem_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_ConsumeItem_Description_TypeInfo> {
            struct Fields {
                uint8_t userPokeID;
                bool isUseActionDisable;
                bool isKinomiCheckDisable;
                bool isConsumeMessageEnable;
                StrParam::Object* successMessage;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_ConsumeItem_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FromEvent_ConsumeItem_Result_TypeInfo> {
            struct Fields {};

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_ConsumeItem_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
