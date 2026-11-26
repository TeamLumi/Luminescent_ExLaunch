#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_PlayWazaEffect : ILClass<Section_FromEvent_PlayWazaEffect, game::Address::Dpr_Battle_Logic_Section_FromEvent_PlayWazaEffect_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_PlayWazaEffect_Description_TypeInfo> {
            struct Fields {
                uint8_t atkPos;
                uint8_t defPos;
                int32_t waza;
                uint8_t wazaType;
                int32_t turnType;
                uint8_t pluralHitIndex;
                bool isSyncEffect;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_PlayWazaEffect_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FromEvent_PlayWazaEffect_Result_TypeInfo> {
            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_PlayWazaEffect_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
