#pragma once

#include "externals/il2cpp-api.h"

#include "externals/BaseEntity.h"
#include "externals/UnityEngine/Renderer.h"
#include "externals/UnityEngine/Transform.h"

struct RendererInfo : ILStruct<RendererInfo> {
    struct Fields {
        UnityEngine::Renderer::Object* _Renderer_k__BackingField;
        int32_t _MaterialCount_k__BackingField;
    };
};
