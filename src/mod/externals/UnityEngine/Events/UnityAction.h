#pragma once

#include "externals/il2cpp.h"
#include "externals/il2cpp-api.h"
#include "externals/System/MulticastDelegate.h"

namespace UnityEngine::Events {
    struct UnityAction : ILClass<UnityAction> {
        struct Fields : System::MulticastDelegate::Fields {

        };

        static const inline game::Address bool_String_TypeInfo = game::Address::UnityEngine_Events_UnityAction_bool_string_TypeInfo;
        static const inline game::Address void_TypeInfo = game::Address::UnityEngine_Events_UnityAction_TypeInfo;
        static const inline game::Address Sprite_TypeInfo = game::Address::UnityEngine_Events_UnityAction_Sprite_TypeInfo;
        static const inline game::Address SettingMenuItem_TypeInfo = game::Address::UnityEngine_Events_UnityAction_SettingMenuItem_TypeInfo;
        static const inline game::Address UIWindow_TypeInfo = game::Address::UnityEngine_Events_UnityAction_UIWindow_TypeInfo;
        static const inline game::Address PokemonPartyItem__int_TypeInfo = game::Address::UnityEngine_Events_UnityAction_PokemonPartyItem_int_TypeInfo;
        static const inline game::Address AudioInstance_TypeInfo = game::Address::UnityEngine_Events_UnityAction_AudioInstance_TypeInfo;

        template <typename T, typename... Args>
        inline void ctor(T* owner, ILMethod<T, Args...>& mi) {
            ctor(owner, *mi);
        }

        // Owner will be the first param of the method it seems
        inline void ctor(void* owner, MethodInfo* mi) {
            external<void>(game::Address::UnityEngine_Events_UnityAction_ctor, this, owner, mi);
        }

        inline void Invoke() {
            external<void>(game::Address::UnityEngine_Events_UnityAction_Invoke, this);
        }

        inline void Invoke(Il2CppObject* arg0) {
            external<void>(game::Address::UnityEngine_Events_UnityAction_AudioData_Invoke, this, arg0);
        }

        inline void Invoke(int32_t arg0, int32_t arg1) {
            external<void>(game::Address::UnityEngine_Events_UnityAction_ConfigID_int_Invoke, this, arg0, arg1);
        }

        inline void ctor() {
            external<void>(game::Address::UnityEngine_Events_UnityAction_ctor, this);
        }

        // DEBUGGING PURPOSES ONLY
        inline uintptr_t getMethodPtrForDebug() {
            return this->instance()->fields.method_ptr - exl::util::modules::GetTargetStart();
        }
    };
}
