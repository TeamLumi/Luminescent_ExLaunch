#pragma once

#include <cstdint>
#include "externals/DPData/CON_PHOTO_DATA.h"

namespace DPData {
    struct CONTEST_DATA : ILStruct<CONTEST_DATA> {
        struct Fields {
            DPData::CON_PHOTO_DATA::Array* photoDatas;
            uint32_t contestRankPoint;
        };
    };
}
