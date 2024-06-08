#pragma once

#include "externals/il2cpp-api.h"

namespace SmartPoint::Components {
    struct PlayerPrefsProvider_ViewerSettings_ : ILClass<PlayerPrefsProvider_ViewerSettings_> {
        struct Fields : ILClass<PlayerPrefsProvider_ViewerSettings_>::Fields {
            bool _loaded;
        };

        template <typename T>
        static inline typename T::Object* get_Instance(ILMethod<T>& method) {
            return external<typename T::Object*>(0x02324910, *method);
        }


    };
}
