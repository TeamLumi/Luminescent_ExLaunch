#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/WazaNo.h"
#include "externals/System/MulticastDelegate.h"

namespace System {
    struct Action : ILClass<Action> {
        struct Fields : System::MulticastDelegate::Fields {

        };

        static const inline game::Address void_TypeInfo = game::Address::System_Action_TypeInfo;
        static const inline game::Address ContextMenuID_TypeInfo = game::Address::System_Action_ContextMenuID_TypeInfo;
        static const inline game::Address WazaNo_WazaNo_TypeInfo = game::Address::System_Action_WazaNo_WazaNo_TypeInfo;
        static const inline game::Address UIZukanRegister_AddMemberResult_TypeInfo = game::Address::System_Action_UIZukanRegister_AddMemberResult_TypeInfo;

        template <typename T, typename... Args>
        inline void ctor(T* target, ILMethod<T, Args...>& mi) {
            ctor(target, *mi);
        }

        inline void ctor(void* target, MethodInfo* delegate) {
            external<void>(game::Address::System_Action_ctor, this, target, delegate);
        }

        inline void Invoke() {
            external<void>(game::Address::System_Action_Invoke, this);
        }

        inline void Invoke(Pml::WazaNo arg0, Pml::WazaNo arg1) {
            external<void>(game::Address::System_Action_Int32Enum_Int32Enum_Invoke, this, arg0, arg1);
        }

        inline void Invoke(Il2CppObject* obj) {
            external<void>(game::Address::System_Action_ADancePlayer_Invoke, this, obj);
        }
    };
}
