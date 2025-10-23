#pragma once
#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/Vector3.h"
#include "externals/UnityEngine/RaycastHit.h"

namespace UnityEngine {
    struct Physics : ILClass<Physics> {
        static inline bool Raycast(UnityEngine::Vector3::Object origin, UnityEngine::Vector3::Object direction,
                                   UnityEngine::RaycastHit::Object* hitInfo, float maxDistance, int32_t layer_mask) {
            UnityEngine::Vector3::Fields originProxy = { .x = origin.fields.x, .y = origin.fields.y, .z = origin.fields.z };
            UnityEngine::Vector3::Fields directionProxy = { .x = direction.fields.x, .y = direction.fields.y, .z = direction.fields.z };
            return external<bool>(0x02c1d320, originProxy, directionProxy, hitInfo, maxDistance, layer_mask);
        }
    };
}