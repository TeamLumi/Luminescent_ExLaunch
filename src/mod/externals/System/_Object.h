#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace System {
    struct _Object : ILClass<_Object> {
        static inline System::String::Object* ToString(Il2CppObject* obj) {
            return external<System::String::Object*>(0x026d13a0, obj);
        }
    };
}
