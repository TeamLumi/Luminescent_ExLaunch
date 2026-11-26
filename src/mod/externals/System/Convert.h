#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace System {
    struct Convert : ILClass<Convert> {

        static inline System::String::Object* ToBase64String(System::Byte_array* inArray) {
            return external<System::String::Object*>(game::Address::System_Convert_ToBase64String, inArray);
        }

        static inline System::Byte_array* FromBase64String(System::String::Object* s) {
            return external<System::Byte_array*>(game::Address::System_Convert_FromBase64String, s);
        }
    };
}
