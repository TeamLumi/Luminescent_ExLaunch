#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/DrivenRectTransformTracker.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/UI/Selectable.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector2.h"

namespace UnityEngine::UI {
    struct Slider : ILClass<Slider> {
        struct Fields : UnityEngine::UI::Selectable::Fields {
            UnityEngine::RectTransform::Object* m_FillRect;
            UnityEngine::RectTransform::Object* m_HandleRect;
            int32_t m_Direction;
            float m_MinValue;
            float m_MaxValue;
            bool m_WholeNumbers;
            float m_Value;
            void* m_OnValueChanged; // UnityEngine_UI_Slider_SliderEvent_o*
            Image::Object* m_FillImage;
            UnityEngine::Transform::Object* m_FillTransform;
            UnityEngine::RectTransform::Object* m_FillContainerRect;
            UnityEngine::Transform::Object* m_HandleTransform;
            UnityEngine::RectTransform::Object* m_HandleContainerRect;
            UnityEngine::Vector2::Object m_Offset;
            UnityEngine::DrivenRectTransformTracker::Object m_Tracker;
            bool m_DelayedUpdateVisuals;
        };

        static_assert(offsetof(Fields, m_DelayedUpdateVisuals) == 0x149);

        inline void set_minValue(float value) {
            external<void>(0x0256d2a0, this, value);
        }

        inline void set_maxValue(float value) {
            external<void>(0x0256d350, this, value);
        }

        inline void set_value(float value) {
            external<void>(0x0256d590, this, value);
        }
    };
}