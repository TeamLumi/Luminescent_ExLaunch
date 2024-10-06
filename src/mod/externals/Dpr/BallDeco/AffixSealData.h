#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::BallDeco {
    struct AffixSealData : ILStruct<AffixSealData> {
        struct Fields {
            uint16_t SealId;
            int16_t PositionX;
            int16_t PositionY;
            int16_t PositionZ;
        };
    };
}
