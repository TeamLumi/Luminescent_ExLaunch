#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/SceneManagement/Scene.h"

namespace UnityEngine::SceneManagement {
    struct SceneManager : ILClass<SceneManager> {
        struct Fields {
            // Seems to be empty
        };

        static inline Scene::Object GetActiveScene() {
            return external<Scene::Object>(game::Address::UnityEngine_SceneManagement_SceneManager_GetActiveScene);
        }
    };
}
