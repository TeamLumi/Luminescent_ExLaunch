#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class MoneyDblUpCause : int32_t {
        MONEY_DBLUP_STD = 0,
        MONEY_DBLUP_EXTRA = 1,
        MONEY_DBLUP_CAUSE_MAX = 2,
    };
}
