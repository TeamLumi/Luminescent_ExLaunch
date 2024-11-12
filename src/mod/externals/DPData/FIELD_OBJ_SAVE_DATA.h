#pragma once

#include "externals/il2cpp-api.h"

namespace DPData {
    struct FIELD_OBJ_SAVE_DATA : ILStruct<FIELD_OBJ_SAVE_DATA> {
        struct Fields {
            uint8_t cnt;
            int32_t name_hash;
            int32_t grid_x;
            int32_t grid_y;
            int32_t height;
            int32_t angle;
            bool active;
            int32_t movecode;
            int32_t dir_head;
            int32_t mvParam0;
            int32_t mvParam1;
            int32_t mvParam2;
            int32_t limitX;
            int32_t limitY;
            int32_t ev_type;
            int32_t mv_old_dir;
            int32_t mv_dir;
        };

        static_assert(offsetof(Fields, mv_dir) == 0x40);
    };
}
