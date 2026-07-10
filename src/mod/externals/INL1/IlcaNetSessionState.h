#pragma once

#include <cstdint>

namespace INL1 {
    enum class IlcaNetSessionState : int32_t {
        SS_None = 0,
        SS_Init = 1,
        SS_MatchingWait = 2,
        SS_GamingFront = 3,
        SS_Gaming = 4,
        SS_GamingLeave = 5,
        SS_GamingError = 6,
        SS_Final = 7,
        SS_Crash = 8,
    };
}
