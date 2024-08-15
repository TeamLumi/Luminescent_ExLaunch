#pragma once

#include "externals/il2cpp-api.h"

namespace DPData {
    struct KinomiGrow : ILStruct<KinomiGrow, 0x04c5a4b0> {
        struct Fields {
            int32_t tagNo;
            int32_t harvestCount;
            int32_t minutes;
            uint8_t _wet;
            uint8_t _selfPlant;
            uint8_t _padding0;
            uint8_t _padding1;
        };
    };
}
