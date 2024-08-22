#pragma once

#include "externals/il2cpp-api.h"

#include "externals/CustomNodeProperty.h"
#include "externals/MaterialParam.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Color.h"

struct CustomNodeMaterial : ILClass<CustomNodeMaterial> {
    struct Fields {
        struct MaterialParam::Object* mp;
        struct System::String::Object* shaderName;
        struct CustomNodeProperty::Array* cnProperties;
        struct UnityEngine::Color::Object fixMultiplyColor;
    };
};