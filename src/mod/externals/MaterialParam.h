#pragma once

#include "externals/il2cpp-api.h"

#include "externals/MaterialUsing.h"
#include "externals/UnityEngine/Material.h"

struct MaterialParam : ILClass<MaterialParam> {
    struct Fields {
        UnityEngine::Material::Object* mat;
        MaterialUsing::Array* usings;
    };
};