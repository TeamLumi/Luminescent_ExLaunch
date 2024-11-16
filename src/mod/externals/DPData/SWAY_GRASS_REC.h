#pragma once
#include "externals/il2cpp-api.h"

namespace DPData {
    struct SWAY_GRASS_REC : ILStruct<SWAY_GRASS_REC, 0x04c5a580> {
        struct Fields {
            int32_t MonsNo;
            int32_t Chain;
        };
    };
}
