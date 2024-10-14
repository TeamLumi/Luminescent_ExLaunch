#pragma once

#include <cstdint>
#include "HONEY_DATA.h"

namespace DPData {
    struct HONEY_TREE : ILStruct<HONEY_TREE> {
        struct Fields {
            int64_t LastUpdateMinutes;
            uint8_t TreeNo;
            HONEY_DATA::Array* HoneyData;
        };
    };
}
