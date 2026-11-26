#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace Pml::PokePara {
    struct InitialSpec : ILClass<InitialSpec, game::Address::Pml_PokePara_InitialSpec_TypeInfo> {
        struct Fields {
            uint64_t randomSeed;
            bool isRandomSeedEnable;
            uint64_t personalRnd;
            uint64_t rareRnd;
            uint64_t id;
            int32_t monsno;
            uint16_t formno;
            uint16_t level;
            uint16_t sex;
            uint16_t seikaku;
            uint8_t tokuseiIndex;
            uint8_t rareTryCount;
            System::UInt16_array* talentPower;
            uint32_t friendship;
            uint8_t talentVNum;
            uint16_t weight;
            uint16_t height;
        };

        inline void ctor() {
            external<void>(game::Address::Pml_PokePara_InitialSpec_ctor, this);
        }
    };
}
