#pragma once

#include <cstdint>

namespace Dpr::NetworkUtils {
    enum class SessionEventType : int32_t {
        Join = 0,
        Leave = 1,
    };
}
