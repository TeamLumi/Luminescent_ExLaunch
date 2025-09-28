#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/Primitives.h"

namespace DPData {
    struct BTLTOWER_CLASSDATA : ILStruct<BTLTOWER_CLASSDATA> {
        struct Fields {
            uint8_t clear_flag;
            bool suspension_flag;
            uint64_t btl_play_rand_seed;
            uint32_t rank;
            uint32_t rank_down_lose;
            int32_t tower_round;
            System::UInt64_array* trainer_random;
            uint32_t total_wins;
            uint32_t total_wins_loop;
            uint32_t total_lose;
            uint32_t old_renshou_cnt;
            uint32_t renshou_cnt;
        };
    };
}
