#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace DPData {
    struct CON_PHOTO_EXT_DATA : ILStruct<CON_PHOTO_EXT_DATA> {
        struct Fields {
            System::String::Object* nickname;
            uint16_t reserved_ushort01;
            int32_t reserved_int01;
        };
    };
}
