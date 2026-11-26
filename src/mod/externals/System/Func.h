#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/MulticastDelegate.h"

namespace System {
    struct Func : ILClass<Func> {
        struct Fields : System::MulticastDelegate::Fields {};

        static const inline game::Address String__SoftwareKeyboard_ErrorState__ValueTuple_bool_String__TypeInfo = game::Address::System_Func_string_SoftwareKeyboard_ErrorState_ValueTuple_bool_string_TypeInfo;
        static const inline game::Address ContextMenuItem_bool__TypeInfo = game::Address::System_Func_ContextMenuItem_bool_TypeInfo;

        template <typename T, typename... Args>
        inline void ctor(T* owner, ILMethod<T, Args...>& mi) {
            ctor(owner, *mi);
        }

        inline void ctor(void* owner, MethodInfo* delegate) {
            external<void>(game::Address::System_Func_int_ctor, this, owner, delegate);
        }
    };
}
