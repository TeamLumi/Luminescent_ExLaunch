#pragma once

#include "externals/il2cpp.h"
#include "externals/il2cpp-api.h"
#include "externals/System/MulticastDelegate.h"

namespace Dpr::UI {
    struct UIWindow;
}

namespace UnityEngine {
    struct Sprite;
}


namespace UnityEngine::Events {
    struct UnityAction : ILClass<UnityAction> {
        struct Fields : System::MulticastDelegate::Fields {

        };

        static const inline long bool_String_TypeInfo = 0x04c5ee10;
        static const inline long void_TypeInfo = 0x04c57230;
        static const inline long Sprite_TypeInfo = 0x04c5e1a0;
        static const inline long SettingMenuItem_TypeInfo = 0x04c62420;
        static const inline long UIWindow_TypeInfo = 0x04c59e60;

        static inline StaticILMethod<0x04c7d030> Method$$Dpr_EvScript_EvDataManager__EvCmdBoxSetProc__b__742_0 {};
        static inline StaticILMethod<0x04c8e658, Dpr::UI::UIWindow> Method$$Invoke$$UIWindow {};
        static inline StaticILMethod<0x04c8e750, UnityEngine::Sprite> Method$$_Sprite_$$Invoke {};

        template <typename T, typename... Args>
        inline void ctor(T* owner, ILMethod<T, Args...>& mi) {
            ctor(owner, *mi);
        }

        // Owner will be the first param of the method it seems
        inline void ctor(void* owner, MethodInfo* mi) {
            external<void>(0x026adeb0, this, owner, mi);
        }

        inline void Invoke() {
            external<void>(0x026a3140, this);
        }

        inline void Invoke(Il2CppObject* arg0) {
            external<void>(0x0253ded0, this, arg0);
        }

        inline void Invoke(int32_t arg0, int32_t arg1) {
            external<void>(0x0253fe60, this, arg0, arg1);
        }

        // Maybe there's a way to generalize this more?
        template <typename T>
        inline void Invoke(T* arg0, ILMethod<T>& mi) {
            external<void>(0x0253ded0, this, arg0, *mi);
        }

        inline void ctor() {
            external<void>(0x026adeb0, this);
        }

        // DEBUGGING PURPOSES ONLY
        inline uintptr_t getMethodPtrForDebug() {
            return this->instance()->fields.method_ptr - exl::util::modules::GetTargetStart();
        }
    };
}
