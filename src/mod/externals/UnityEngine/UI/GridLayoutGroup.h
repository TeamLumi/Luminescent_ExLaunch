#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/UI/LayoutGroup.h"
#include "externals/UnityEngine/Vector2.h"

namespace UnityEngine::UI {
    struct GridLayoutGroup : ILClass<GridLayoutGroup> {
        struct Fields : UnityEngine::UI::LayoutGroup::Fields {
            int32_t m_StartCorner;
            int32_t m_StartAxis;
            UnityEngine::Vector2::Object m_CellSize;
            UnityEngine::Vector2::Object m_Spacing;
            int32_t m_Constraint;
            int32_t m_ConstraintCount;
        };
    };
}