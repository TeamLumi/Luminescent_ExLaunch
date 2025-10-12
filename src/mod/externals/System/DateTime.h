#pragma once

#include "externals/il2cpp-api.h"

namespace System {
    struct DateTime : ILStruct<DateTime> {
        struct Fields {
            uint64_t dateData;
        };
    };
}