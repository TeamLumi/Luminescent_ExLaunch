#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::NetworkUtils {
    struct RequestValidateResult : ILStruct<RequestValidateResult> {
        struct Fields {
            int32_t callbackResult;
            int32_t validateResult;
        };
    };
}
