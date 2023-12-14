#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/EventSystems/UIBehaviour.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/Vector3.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/Bounds.h"
#include "externals/UnityEngine/DrivenRectTransformTracker.h"
#include "externals/UnityEngine/RectOffset.h"

namespace UnityEngine::UI {
    struct LayoutGroup : ILClass<LayoutGroup> {
        struct Fields : UnityEngine::EventSystems::UIBehaviour::Fields {
            UnityEngine::RectOffset::Object* m_Padding;
            int32_t m_ChildAlignment;
            UnityEngine::RectTransform::Object* m_Rect;
            UnityEngine::DrivenRectTransformTracker::Object m_Tracker;
            UnityEngine::Vector2::Object m_TotalMinSize;
            UnityEngine::Vector2::Object m_TotalPreferredSize;
            UnityEngine::Vector2::Object m_TotalFlexibleSize;
            System::Collections::Generic::List$$RectTransform::Object* m_RectChildren;
        };

        static_assert(sizeof(Fields) == 0x48);
    };
}
