#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Component.h"

namespace UnityEngine {
    struct Collider : ILClass<Collider, game::Address::UnityEngine_Collider_TypeInfo> {
        struct Fields : UnityEngine::Component::Fields {

        };

        inline void set_enabled(bool value) {
            external<void>(game::Address::UnityEngine_Collider_set_enabled, this, value);
        }
    };
}
