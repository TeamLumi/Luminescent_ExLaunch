#pragma once

#include "externals/il2cpp-api.h"


// Forward declaration to avoid circular dependencies
struct PlayerWork;

namespace SmartPoint::Components {
    template <typename T>
    struct PlayerPrefsProvider : ILClass<T> {
        struct Fields {
            bool _loaded;
        };

        struct VirtualInvokeData_get_key {
            typedef System::String::Object*(*Il2CppMethodPointer)(T::Object*, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VTable {
            VirtualInvokeData _0_Equals;
            VirtualInvokeData _1_Finalize;
            VirtualInvokeData _2_GetHashCode;
            VirtualInvokeData _3_ToString;
            VirtualInvokeData_get_key _4_get_key;
            VirtualInvokeData _5_Initialization;
            VirtualInvokeData _6_CustomLoadOperation;
            VirtualInvokeData _7_CustomSaveOperation;
            VirtualInvokeData _8_CustomLoadAsyncOperation;
            VirtualInvokeData _9_CustomSaveAsyncOperation;
            VirtualInvokeData _10_OnPostLoad;
            VirtualInvokeData _11_OnPreSave;
        };

        static inline StaticILMethod<0x04c90330, PlayerWork>
                Method$SmartPoint_Components_PlayerPrefsProvider_PlayerWork_get_instance {};

        static inline StaticILMethod<0x04c82110, PlayerWork>
                Method$SmartPoint_Components_PlayerPrefsProvider_PlayerWork_Save {};

        static inline typename T::Object* get_Instance(ILMethod<T>& method) {
            return ILClass<T>::template external<typename T::Object*>(0x02324910, *method);
        }
    };
}