#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace DPData {
    struct TV_STR_DATA : ILStruct<TV_STR_DATA> {
        struct Fields {
            System::String::Object* value;
            uint8_t language;
            uint8_t genderId;
            uint8_t reserved2;
            uint8_t reserved3;
        };
    };
}
