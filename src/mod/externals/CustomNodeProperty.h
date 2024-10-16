#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Vector2.h"

struct CustomNodeProperty : ILClass<CustomNodeProperty> {
    struct Fields {
        UnityEngine::GameObject::Object* bone;
        UnityEngine::GameObject::Object* boneSub;
        System::String::Object* propertyName;
        int32_t pType;
        UnityEngine::Vector2::Object baseUV;
        UnityEngine::Vector2::Object baseScale;
        System::String::Object* propertyName_U;
        System::String::Object* propertyName_V;
    };
};