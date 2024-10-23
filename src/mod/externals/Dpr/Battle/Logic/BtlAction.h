#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class BtlAction : int32_t {
        BTL_ACTION_NULL = 0,
        BTL_ACTION_FIGHT = 1,
        BTL_ACTION_ITEM = 2,
        BTL_ACTION_CHANGE = 3,
        BTL_ACTION_ESCAPE = 4,
        BTL_ACTION_SKIP = 5,
        BTL_ACTION_G_START = 6,
        BTL_ACTION_CHEER = 7,
        BTL_ACTION_RECPLAY_TIMEOVER = 8,
        BTL_ACTION_RECPLAY_ERROR = 9,
        BTL_ACTION_SAFARI_BALL = 10,
        BTL_ACTION_SAFARI_ESA = 11,
        BTL_ACTION_SAFARI_DORO = 12,
        BTL_ACTION_SAFARI_YOUSUMI = 13,
    };
}