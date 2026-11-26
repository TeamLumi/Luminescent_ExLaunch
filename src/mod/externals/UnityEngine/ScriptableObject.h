#pragma once

#include "externals/UnityEngine/_Object.h"

namespace UnityEngine {
    struct ScriptableObject : ILClass<ScriptableObject, game::Address::UnityEngine_ScriptableObject_TypeInfo> {
        struct Fields : UnityEngine::_Object::Fields {

        };
    };
}
