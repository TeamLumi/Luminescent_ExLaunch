#pragma once

#include "externals/System/String.h"
#include "externals/il2cpp-api.h"

namespace System {
    struct Int32Class : ILStruct<Int32Class> {
        struct Fields {
            int32_t m_value;
        };

        static inline bool TryParse(System::String::Object* s, int32_t* result) {
            return external<bool>(0x022a7e80, s, result);
        }

        // Not technically a static method but it's easier to deal with it like this
        static inline System::String::Object* ToString(int32_t* __this, System::String::Object* format) {
            return external<System::String::Object*>(0x022a7c10, __this, format);
        }
    };
}