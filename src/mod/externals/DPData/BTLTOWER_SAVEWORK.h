#pragma once

#include <cstdint>
#include "externals/DPData/BTLTOWER_CLASSDATA.h"

namespace DPData {
    struct BTLTOWER_SAVEWORK : ILStruct<BTLTOWER_SAVEWORK> {
        struct Fields {
            uint32_t max_master_rank;
            int32_t play_mode;
            int32_t old_playmode;
            uint32_t btl_point;
            uint32_t day_challeng_cnt;
            DPData::BTLTOWER_CLASSDATA::Array* class_data;
            uint32_t challenge_cnt;
        };
    };
}
