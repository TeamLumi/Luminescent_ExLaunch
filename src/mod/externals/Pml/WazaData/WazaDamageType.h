#pragma once

#include "externals/il2cpp-api.h"

namespace Pml::WazaData {
    enum class WazaDamageType : int32_t {
        NONE = 0,
        PHYSIC = 1,
        SPECIAL = 2,
        MAX = 3,
    };
}