#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Behaviour.h"

namespace UnityEngine {
    struct Camera : ILClass<Camera> {
        struct Fields : UnityEngine::Behaviour::Fields {};

        inline float get_fieldOfView() {
            return external<float>(game::Address::UnityEngine_Camera_get_fieldOfView, this);
        }
    };
}