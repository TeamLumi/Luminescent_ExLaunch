#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Behaviour.h"

namespace UnityEngine {
    struct Camera : ILClass<Camera> {
        struct Fields : UnityEngine::Behaviour::Fields {};

        inline float get_fieldOfView() {
            return external<float>(0x026a4920, this);
        }

        inline void set_aspect(float value) {
            external<void>(0x026a4ce0, this, value);
        }
    };
}