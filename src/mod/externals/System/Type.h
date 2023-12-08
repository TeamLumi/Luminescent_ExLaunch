#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/RuntimeTypeHandle.h"
#include "externals/System/String.h"

namespace System {
    struct Type : ILClass<Type> {
        struct Fields {
//            System::Reflection::MemberInfo::Fields super;  // Empty struct
            System::RuntimeTypeHandle::Object _impl;
        };

        static System::Type::Object* GetTypeFromHandle(System::RuntimeTypeHandle::Object handle) {
            return external<System::Type::Object*>(0x02b194d0, handle);
        }

        static System::Type::Object* GetType(System::String::Object* typeName) {
            return external<System::Type::Object*>(0x02b1e1d0, typeName);
        }
    };
}
