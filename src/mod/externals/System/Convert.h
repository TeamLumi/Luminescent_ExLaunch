#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace System {
    struct Convert : ILClass<Convert> {

        static inline System::String::Object* ToBase64String(System::Byte_array* inArray) {
            return external<System::String::Object*>(0x024ee6e0, inArray);
        }
    };
}
