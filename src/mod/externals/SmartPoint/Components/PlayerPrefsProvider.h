#pragma once

#include "externals/il2cpp-api.h"

// Forward declaration to avoid circular dependencies
struct ViewerSettings;

namespace SmartPoint::Components {
    template <typename T>
    struct PlayerPrefsProvider : ILClass<T> {
        struct Fields {
            bool _loaded;
        };

        static inline StaticILMethod<0x04c90330, ViewerSettings>
                Method$SmartPoint_Components_PlayerPrefsProvider_PlayerWork_get_instance;

        static inline typename T::Object* get_Instance(ILMethod<T>& method) {
            return ILClass<T>::template external<typename T::Object*>(0x02324910, *method);
        }
    };
}