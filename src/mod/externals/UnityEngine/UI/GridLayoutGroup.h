#pragma once
#include "externals/il2cpp-api.h"
#include "LayoutGroup.h"

namespace UnityEngine::UI {
    struct GridLayoutGroup : ILClass<GridLayoutGroup> {
        struct Fields : UnityEngine::UI::LayoutGroup::Fields {
            int32_t m_StartCorner;
            int32_t m_StartAxis;
            Vector2::Object m_CellSize;
            Vector2::Object m_Spacing;
            int32_t m_Constraint;
            int32_t m_ConstraintCount;
        };

        inline void set_cellSize(Vector2::Object value) {
            Vector2::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y };
            external<void>(0x024b7130, this, valueProxy);
        };
    };
}