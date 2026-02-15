#pragma once

#include "externals/il2cpp-api.h"

namespace SmartPoint::Components {
    struct PlayerPrefsProvider : ILClass<PlayerPrefsProvider> {
        struct Fields {
            bool _loaded;
        };

        struct VTable {
            VirtualInvokeData _0_Equals;
            VirtualInvokeData _1_Finalize;
            VirtualInvokeData _2_GetHashCode;
            VirtualInvokeData _3_ToString;
            VirtualInvokeData _4_get_key;
            VirtualInvokeData _5_Initialization;
            VirtualInvokeData _6_CustomLoadOperation;
            VirtualInvokeData _7_CustomSaveOperation;
            VirtualInvokeData _8_CustomLoadAsyncOperation;
            VirtualInvokeData _9_CustomSaveAsyncOperation;
            VirtualInvokeData _10_OnPostLoad;
            VirtualInvokeData _11_OnPreSave;
        };

        template <typename T>
        static inline typename T::Object* get_instance(ILMethod<T>& method) {
            return external<typename T::Object*>(0x02324910, *method);
        }
    };
}