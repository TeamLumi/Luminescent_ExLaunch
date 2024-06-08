#pragma once

#include "externals/il2cpp-api.h"
#include "externals/ViewerSettings.h"

namespace SmartPoint::Components {
    struct PlayerPrefsProvider_PlayerWork_ : ILClass<PlayerPrefsProvider_PlayerWork_> {
        struct Fields : ILClass<PlayerPrefsProvider_PlayerWork_>::Fields {
            bool _loaded;
        };

        static inline StaticILMethod<0x04c90330, ViewerSettings>
            Method$SmartPoint_Components_PlayerPrefsProvider_PlayerWork_get_instance;
    };
}
