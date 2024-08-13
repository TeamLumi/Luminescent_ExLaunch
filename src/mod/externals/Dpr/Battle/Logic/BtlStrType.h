#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class BtlStrType : int32_t {
        BTL_STRTYPE_NULL = 0,
        BTL_STRTYPE_STD = 1,
        BTL_STRTYPE_SET = 2,
        BTL_STRTYPE_WAZA = 3,
        BTL_STRTYPE_YESNO = 4,
        BTL_STRTYPE_MAX = 5,
    };
}