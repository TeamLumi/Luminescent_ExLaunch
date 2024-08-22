#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Renderer.h"

struct MaterialUsing : ILClass<MaterialUsing> {
    struct Fields {
        UnityEngine::Renderer::Object* renderer;
        int32_t index;
    };
};