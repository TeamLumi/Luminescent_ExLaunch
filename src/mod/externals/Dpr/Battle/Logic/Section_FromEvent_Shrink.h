#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/RankEffectCause.h"
#include "externals/Dpr/Battle/Logic/RankEffectViewType.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Pml/WazaData/WazaRankEffect.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_Shrink : ILClass<Section_FromEvent_Shrink, game::Address::Dpr_Battle_Logic_Section_FromEvent_Shrink_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_Shrink_Description_TypeInfo> {
            struct Fields {
                uint8_t pokeID;
                uint8_t percentage;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_Shrink_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FromEvent_Shrink_Result_TypeInfo> {
            struct Fields {
                bool isSucceeded;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_Shrink_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
