#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/Vector3.h"
#include "externals/UnityEngine/Vector2.h"

namespace UnityEngine {
    struct RaycastHit : ILStruct<RaycastHit> {
        struct Fields {
            UnityEngine::Vector3::Object m_Point;
            UnityEngine::Vector3::Object m_Normal;
            uint32_t m_FaceID;
            float m_Distance;
            UnityEngine::Vector2::Object m_UV;
            int32_t m_Collider;
        };

        static_assert(offsetof(Fields, m_Collider) == 0x28);
    };
}