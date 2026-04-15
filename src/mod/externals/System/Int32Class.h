#pragma once

#include "externals/System/String.h"
#include "externals/il2cpp-api.h"

namespace System {
    struct Int32Class : ILStruct<Int32Class> {
        struct Fields {
            int32_t thisshouldsayint32_tapparentlybutitdoesntbuildthatwaysocheckwithsmathatanothernameisokaylater;
        };

        static inline bool TryParse(System::String::Object* s, int32_t* result) {
            return external<bool>(0x022a7e80, s, result);
        }
    };
}