#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/MulticastDelegate.h"

namespace System {
    struct Func : ILClass<Func> {
        struct Fields : System::MulticastDelegate::Fields {

        };

        static const inline long String__SoftwareKeyboard_ErrorState__ValueTuple_bool_String__TypeInfo = 0x04c5ee08;
        static const inline long ContextMenuItem__bool__TypeInfo = 0x04c5e790;

        static inline StaticILMethod<0x04c8baf8> Method$$System_Func__ContextMenuItem__bool__ctor {};

        template <typename T, typename... Args>
        inline void ctor(T* owner, ILMethod<T, Args...>& mi) {
            ctor(owner, *mi);
        }

        inline void ctor(void* owner, MethodInfo* delegate) {
            external<void>(0x0283f570, this, owner, delegate);
        }
    };
}
