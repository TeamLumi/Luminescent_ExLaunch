#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Behaviour.h"

namespace UnityEngine {
    struct Camera : ILClass<Camera> {
        struct Fields : UnityEngine::Behaviour::Fields {};
    };
}