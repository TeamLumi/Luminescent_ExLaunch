#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Collider.h"

namespace UnityEngine {
    struct BoxCollider : ILClass<BoxCollider, game::Address::UnityEngine_BoxCollider_TypeInfo> {
        struct Fields : UnityEngine::Collider::Fields {

        };
    };
}
