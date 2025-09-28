#pragma once

#include "externals/il2cpp-api.h"

namespace DPData {
    struct TOPMENUITEM_WORK : ILStruct<TOPMENUITEM_WORK> {
        struct Fields {
            int32_t index;
            bool isNew;
        };

        static_assert(offsetof(Fields, isNew) == 0x4);
    };
}
