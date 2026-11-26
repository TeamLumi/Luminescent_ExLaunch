#pragma once

#include <common.hpp>

namespace exl::util {

    enum class UserVersion {
        DEFAULT,

    };

    namespace impl {
        ALWAYS_INLINE UserVersion DetermineUserVersion() {
            
            return UserVersion::DEFAULT;
        }
    }
}
