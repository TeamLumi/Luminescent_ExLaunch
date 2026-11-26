#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_SetWazaEffectEnable : ILClass<Section_FromEvent_SetWazaEffectEnable, game::Address::Dpr_Battle_Logic_Section_FromEvent_SetWazaEffectEnable_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_SetWazaEffectEnable_Description_TypeInfo> {
            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_SetWazaEffectEnable_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FromEvent_SetWazaEffectEnable_Result_TypeInfo> {
            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_SetWazaEffectEnable_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
