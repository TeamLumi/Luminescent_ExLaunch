#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/MulticastDelegate.h"

namespace System {
    struct Predicate : ILClass<Predicate> {
        struct Fields : System::MulticastDelegate::Fields {};

        static const inline long PoffinData__TypeInfo = 0x04c5a530;

        template <typename T, typename... Args>
        inline void ctor(T* owner, ILMethod<T, Args...>& mi) {
            ctor(owner, *mi);
        }

        inline void ctor(void* owner, MethodInfo* delegate) {
            external<void>(0x025e3b60, this, owner, delegate);
        }
    };
}
