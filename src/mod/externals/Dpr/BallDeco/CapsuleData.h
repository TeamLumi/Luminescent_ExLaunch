#pragma once

#include <cstdint>
#include "AffixSealData.h"

namespace Dpr::BallDeco {
    struct CapsuleData : ILStruct<CapsuleData> {
        struct Fields {
            uint32_t AttachPokemonId;
            uint32_t AttachPersonalRnd;
            bool Is3DEditMode;
            bool IsAppliedTemplate;
            uint8_t AffixSealCount;
            Dpr::BallDeco::AffixSealData::Array* AffixSealDatas;
        };
    };
}
