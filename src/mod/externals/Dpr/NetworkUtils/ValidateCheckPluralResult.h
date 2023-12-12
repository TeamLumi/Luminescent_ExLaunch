#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::NetworkUtils {
    struct ValidateCheckPluralResult : ILStruct<ValidateCheckPluralResult> {
        struct Fields {
            void* illegalList; // System_Collections_Generic_List_bool__o*
            int32_t validateResult;
            bool bIsSuccess;
        };
    };
}
