#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::BallDeco {
    struct SaveSealData : ILStruct<SaveSealData> {
        struct Fields {
            bool IsGet;
            int32_t Count;
            int32_t TotalCount;
        };
    };
}
