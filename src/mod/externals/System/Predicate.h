#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/MulticastDelegate.h"

namespace System {
    struct Predicate : ILClass<Predicate> {
        struct Fields : System::MulticastDelegate::Fields {};

        static const inline game::Address PoffinData__TypeInfo = game::Address::System_Predicate_PoffinData_TypeInfo;

        template <typename T, typename... Args>
        inline void ctor(T* owner, ILMethod<T, Args...>& mi) {
            ctor(owner, *mi);
        }

        inline void ctor(void* owner, MethodInfo* delegate) {
            external<void>(game::Address::System_Predicate_PoffinData_ctor, this, owner, delegate);
        }
    };
}
