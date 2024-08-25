#pragma once

#include <cstdint>
#include "externals/il2cpp-api.h"

namespace DPData {
    struct LOCATION_WORK : ILStruct<LOCATION_WORK> {
        struct Fields {
            int32_t zoneID;
            float posX;
            float posY;
            float posZ;
            int32_t dir;
        };
    };
}
