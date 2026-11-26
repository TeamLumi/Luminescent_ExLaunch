#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"
#include "externals/Dpr/Battle/Logic/Section_CommonParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_SetWazaEffectIndex : ILClass<Section_FromEvent_SetWazaEffectIndex, game::Address::Dpr_Battle_Logic_Section_FromEvent_SetWazaEffectIndex_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_SetWazaEffectIndex_Description_TypeInfo> {
            struct Fields {
                uint8_t effectIndex;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_SetWazaEffectIndex_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FromEvent_SetWazaEffectIndex_Result_TypeInfo> {
            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_SetWazaEffectIndex_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
