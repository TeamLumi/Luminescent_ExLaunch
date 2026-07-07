#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::BallDeco {
    // One seal sticker on a ball capsule. 8-byte value struct (stride 8 in
    // AffixSealData[]). Array TypeInfo = Dpr.BallDeco.AffixSealData[]_TypeInfo.
    struct AffixSealData : ILStruct<AffixSealData, 0x04c59f28> {
        struct Fields {
            uint16_t SealId;
            int16_t PositionX;
            int16_t PositionY;
            int16_t PositionZ;
        };
    };

    static_assert(sizeof(AffixSealData::Object) == 0x8, "AffixSealData must be 8 bytes");
}
