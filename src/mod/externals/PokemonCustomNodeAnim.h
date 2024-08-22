#pragma once

#include "externals/il2cpp-api.h"

#include "externals/CustomNodeMaterial.h"
#include "externals/CustomNodeVisibility.h"
#include "externals/UnityEngine/MonoBehaviour.h"

struct PokemonCustomNodeAnim : ILClass<PokemonCustomNodeAnim> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        bool debugKey;
        CustomNodeMaterial::Array* mCustomNodeMaterials;
        CustomNodeVisibility::Array* mCNViss;
        bool isAutoUpdate;
        bool _isSetup;
    };
};