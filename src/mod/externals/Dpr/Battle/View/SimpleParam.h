#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::View {
    struct SimpleParam : ILStruct<SimpleParam> {
        struct Fields {
            int32_t monsNo;
            int32_t formNo;
            uint8_t sex;
            bool isRare;
            bool isEgg;
            float scale;
            int32_t perRand;
            int32_t gType;
            int32_t adjustHeight;
            int32_t speciesValue;
        };
    };
}