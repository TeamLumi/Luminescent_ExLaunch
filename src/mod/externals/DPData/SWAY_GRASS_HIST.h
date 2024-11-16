#pragma once
#include "SWAY_GRASS_REC.h"

namespace DPData {
    struct SWAY_GRASS_HIST : ILStruct<SWAY_GRASS_HIST> {
        struct Fields {
            SWAY_GRASS_REC::Array* SwayGrassRec;
        };
    };
}
