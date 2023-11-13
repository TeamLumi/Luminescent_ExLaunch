#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/RankEffectCause.h"
#include "externals/Dpr/Battle/Logic/RankEffectViewType.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Pml/WazaData/WazaRankEffect.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_PlayWazaEffect : ILClass<Section_FromEvent_PlayWazaEffect, 0x04c5b9a0> {
        struct Description : ILClass<Description, 0x04c5b380> {
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
                external<void>(0x01fb6110, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5afd8> {
            inline void ctor() {
                external<void>(0x01fb6150, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
