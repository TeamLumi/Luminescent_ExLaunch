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

        struct VirtualInvokeData_set_value {
            typedef void(*Il2CppMethodPointer)(Slider::Object*, float, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VTable {
            VirtualInvokeData _0_Equals;
            VirtualInvokeData _1_Finalize;
            VirtualInvokeData _2_GetHashCode;
            VirtualInvokeData _3_ToString;
            VirtualInvokeData _4_Awake;
            VirtualInvokeData _5_OnEnable;
            VirtualInvokeData _6_Start;
            VirtualInvokeData _7_OnDisable;
            VirtualInvokeData _8_OnDestroy;
            VirtualInvokeData _9_IsActive;
            VirtualInvokeData _10_OnRectTransformDimensionsChange;
            VirtualInvokeData _11_OnBeforeTransformParentChanged;
            VirtualInvokeData _12_OnTransformParentChanged;
            VirtualInvokeData _13_OnDidApplyAnimationProperties;
            VirtualInvokeData _14_OnCanvasGroupChanged;
            VirtualInvokeData _15_OnCanvasHierarchyChanged;
            VirtualInvokeData _16_IsDestroyed;
            VirtualInvokeData _17_unknown;
            VirtualInvokeData _18_unknown;
            VirtualInvokeData _19_unknown;
            VirtualInvokeData _20_unknown;
            VirtualInvokeData _21_unknown;
            VirtualInvokeData _22_unknown;
            VirtualInvokeData _23_unknown;
            VirtualInvokeData _24_IsInteractable;
            VirtualInvokeData _25_InstantClearState;
            VirtualInvokeData _26_DoStateTransition;
            VirtualInvokeData _27_FindSelectableOnLeft;
            VirtualInvokeData _28_FindSelectableOnRight;
            VirtualInvokeData _29_FindSelectableOnUp;
            VirtualInvokeData _30_FindSelectableOnDown;
            VirtualInvokeData _31_OnMove;
            VirtualInvokeData _32_OnPointerDown;
            VirtualInvokeData _33_OnPointerUp;
            VirtualInvokeData _34_OnPointerEnter;
            VirtualInvokeData _35_OnPointerExit;
            VirtualInvokeData _36_OnSelect;
            VirtualInvokeData _37_OnDeselect;
            VirtualInvokeData _38_Select;
            VirtualInvokeData _39_unknown;
            VirtualInvokeData _40_unknown;
            VirtualInvokeData _41_unknown;
            VirtualInvokeData _42_UnityEngine_UI_ICanvasElement_get_transform;
            VirtualInvokeData _43_unknown;
            VirtualInvokeData _44_unknown;
            VirtualInvokeData _45_unknown;
            VirtualInvokeData _46_get_value;
            VirtualInvokeData_set_value _47_set_value;
            VirtualInvokeData _48_SetValueWithoutNotify;
            VirtualInvokeData _49_Rebuild;
            VirtualInvokeData _50_LayoutComplete;
            VirtualInvokeData _51_GraphicUpdateComplete;
            VirtualInvokeData _52_Update;
            VirtualInvokeData _53_Set;
            VirtualInvokeData _54_OnDrag;
            VirtualInvokeData _55_OnInitializePotentialDrag;
        };

        static_assert(offsetof(Fields, m_DelayedUpdateVisuals) == 0x149);

        inline void virtual_set_value(float value) {
            (*(this->instance()->klass->vtable)._47_set_value.methodPtr)
                    (this->instance(), value, this->instance()->klass->vtable._47_set_value.method);
        }

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