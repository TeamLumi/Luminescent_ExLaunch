#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/MulticastDelegate.h"

namespace System {
    struct Action : ILClass<Action> {
        struct Fields : System::MulticastDelegate::Fields {

        };

        static const inline long void_TypeInfo = 0x04c56040;
        static const inline long ContextMenuID_TypeInfo = 0x04c627e0;
        static const inline long WazaNo_WazaNo_TypeInfo = 0x04c5eff0;

        template <typename T, typename... Args>
        inline void ctor(T* target, ILMethod<T, Args...>& mi) {
            ctor(target, *mi);
        }

        inline void ctor(void* target, MethodInfo* delegate) {
            external<void>(0x023feb30, this, target, delegate);
        }

        inline void Invoke() {
            external<void>(0x023feb50, this);
        }
    };
}
