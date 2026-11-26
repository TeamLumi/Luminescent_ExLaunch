#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Renderer.h"

namespace UnityEngine {
    struct SkinnedMeshRenderer : ILClass<SkinnedMeshRenderer, game::Address::UnityEngine_SkinnedMeshRenderer_TypeInfo> {
        struct Fields : UnityEngine::Renderer::Fields {
        };
    };
}
