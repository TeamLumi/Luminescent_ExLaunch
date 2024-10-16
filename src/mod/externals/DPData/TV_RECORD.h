#pragma once

#include "externals/DPData/TV_STR_DATA.h"

namespace DPData {
    struct TV_RECORD : ILStruct<TV_RECORD> {
        struct Fields {
            System::Int32_array* int_data;
            DPData::TV_STR_DATA::Array* str_data;
        };
    };
}
