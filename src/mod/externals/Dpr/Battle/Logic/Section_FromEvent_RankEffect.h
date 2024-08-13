#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/RankEffectCause.h"
#include "externals/Dpr/Battle/Logic/RankEffectViewType.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Pml/WazaData/WazaRankEffect.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_RankEffect : ILClass<Section_FromEvent_RankEffect, 0x04c5b9c0> {
        struct Description : ILClass<Description, 0x04c5b1b8> {
            struct Fields {
                uint8_t pokeID;
                uint8_t targetPokeCount;
                System::Byte_array* targetPokeID;
                Pml::WazaData::WazaRankEffect rankType;
                int8_t rankVolume;
                RankEffectCause cause;
                uint16_t itemID;
                uint32_t effectSerial;
                bool isDisplayTokuseiWindow;
                bool isStandardMessageDisable;
                bool isSpFailMessageDisplay;
                bool byWazaEffect;
                bool isPreEffectMessageEnable;
                RankEffectViewType effectViewType;
                bool isMigawariThrew;
                void* message; // Dpr_Battle_Logic_StrParam_o*
            };

            inline void ctor() {
                external<void>(0x01fb6e30, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
