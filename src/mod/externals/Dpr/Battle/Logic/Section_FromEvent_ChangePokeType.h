#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeTypePair.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_ChangePokeType : ILClass<Section_FromEvent_ChangePokeType, game::Address::Dpr_Battle_Logic_Section_FromEvent_ChangePokeType_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_ChangePokeType_Description_TypeInfo> {
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
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_ChangePokeType_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FromEvent_ChangePokeType_Result_TypeInfo> {
            struct Fields {
                bool isSuccessed;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_ChangePokeType_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
