#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Renderer.h"

struct CustomNodeVisibility : ILClass<CustomNodeVisibility> {
    struct Fields {
        UnityEngine::GameObject::Object* bone;
        UnityEngine::Renderer::Object* skin;
    };
};