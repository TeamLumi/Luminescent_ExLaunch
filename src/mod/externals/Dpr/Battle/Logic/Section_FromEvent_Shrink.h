#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/RankEffectCause.h"
#include "externals/Dpr/Battle/Logic/RankEffectViewType.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Pml/WazaData/WazaRankEffect.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_Shrink : ILClass<Section_FromEvent_Shrink, 0x04c5ba60> {
        struct Description : ILClass<Description, 0x04c5b1e8> {
            struct Fields {
                uint8_t pokeID;
                uint8_t percentage;
            };

            inline void ctor() {
                external<void>(0x01fb8490, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5af00> {
            struct Fields {
                bool isSucceeded;
            };

            inline void ctor() {
                external<void>(0x01fb84c0, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
