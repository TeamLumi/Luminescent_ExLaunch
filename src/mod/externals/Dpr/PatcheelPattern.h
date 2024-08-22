#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/SkinnedMeshRenderer.h"
#include "externals/UnityEngine/Vector2.h"

namespace Dpr {
    struct PatcheelPattern : ILClass<PatcheelPattern> {
        struct UVData : ILClass<UVData> {
            UnityEngine::SkinnedMeshRenderer::Object* renderer;
            UnityEngine::Vector2::Array* UVs;
            int32_t value;
        };

        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UVData::Array* UVDatas;
        };
    };
}