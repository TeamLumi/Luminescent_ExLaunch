#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::NetworkUtils {
    struct ValidateCheckResult : ILStruct<ValidateCheckResult> {
        struct Fields {
            int32_t validateResult;
            bool bIsSuccess;
        };
    };
}
