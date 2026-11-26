#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/GameObject.h"

namespace UnityEngine::SceneManagement {
    struct Scene : ILStruct<Scene> {
        struct Fields {
            int32_t m_Handle;
        };

        inline UnityEngine::GameObject::Array* GetRootGameObjects() {
            return external<UnityEngine::GameObject::Array*>(game::Address::UnityEngine_SceneManagement_Scene_GetRootGameObjects, this);
        }

        inline System::String::Object* get_name() {
            return external<System::String::Object*>(game::Address::UnityEngine_SceneManagement_Scene_get_name, this);
        }
    };
}
