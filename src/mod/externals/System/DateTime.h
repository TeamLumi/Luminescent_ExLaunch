#pragma once

#include "externals/il2cpp-api.h"

namespace System {
    struct DateTime : ILStruct<DateTime> {
        struct Fields {
            uint64_t dateData;
        };

        inline int32_t get_Year() {
            return external<int32_t>(0x024f32c0, this);
        }

        inline int32_t get_Month() {
            return external<int32_t>(0x024f2ff0, this);
        }

        inline int32_t get_Day() {
            return external<int32_t>(0x024f2ee0, this);
        }
    };
}