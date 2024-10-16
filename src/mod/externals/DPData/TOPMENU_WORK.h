#pragma once

#include "externals/il2cpp-api.h"
#include "TOPMENUITEM_WORK.h"

namespace DPData {
    struct TOPMENU_WORK : ILStruct<TOPMENU_WORK> {
        struct Fields {
            TOPMENUITEM_WORK::Array* items;
            int32_t selectType;
        };
    };
}
