#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/BallDeco/CapsuleData.h"

namespace Dpr::BallDeco {
    struct SaveBallDecoData : ILStruct<SaveBallDecoData> {
        struct Fields {
            uint8_t CapsuleCount;
            Dpr::BallDeco::CapsuleData::Array* CapsuleDatas;
        };
    };
}
