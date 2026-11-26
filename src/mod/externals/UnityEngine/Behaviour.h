#pragma once

#include "externals/il2cpp.h"
#include "externals/UnityEngine/Component.h"
#include "Component.h"

namespace UnityEngine {
    struct Behaviour : ILClass<Behaviour> {
        struct Fields : UnityEngine::Component::Fields {

        };

        inline void set_enabled(bool value) {
            external<void>(game::Address::UnityEngine_Behaviour_set_enabled, this, value);
        }

        inline bool get_enabled() {
            return external<bool>(game::Address::UnityEngine_Behaviour_get_enabled, this);
        }
    };
}
