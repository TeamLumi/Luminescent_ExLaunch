#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Collections/Generic/Dictionary.h"
#include "externals/UnityEngine/Sprite.h"
#include "externals/UnityEngine/UI/MaskableGraphic.h"

namespace UnityEngine::UI {
    struct Image : ILClass<Image, 0, 0x04c63db8> {
        struct Fields : UnityEngine::UI::MaskableGraphic::Fields {
            Sprite::Object* m_Sprite;
            Sprite::Object* m_OverrideSprite;
            int32_t m_Type;
            bool m_PreserveAspect;
            bool m_FillCenter;
            int32_t m_FillMethod;
            float m_FillAmount;
            bool m_FillClockwise;
            int32_t m_FillOrigin;
            float m_AlphaHitTestMinimumThreshold;
            bool m_Tracked;
            bool m_UseSpriteMesh;
            float m_PixelsPerUnitMultiplier;
            float m_CachedReferencePixelsPerUnit;
        };

        struct VirtualInvokeData_set_color {
            typedef void(*Il2CppMethodPointer)(UnityEngine::UI::Image::Object* __this, UnityEngine::Color::Fields value, const MethodInfo*);
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
            VirtualInvokeData _18_UnityEngine_UI_ICanvasElement_get_transform;
            VirtualInvokeData _19_unknown;
            VirtualInvokeData _20_unknown;
            VirtualInvokeData _21_unknown;
            VirtualInvokeData _22_get_color;
            VirtualInvokeData_set_color _23_set_color;
            VirtualInvokeData _24_get_raycastTarget;
            VirtualInvokeData _25_set_raycastTarget;
            VirtualInvokeData _26_SetAllDirty;
            VirtualInvokeData _27_SetLayoutDirty;
            VirtualInvokeData _28_SetVerticesDirty;
            VirtualInvokeData _29_SetMaterialDirty;
            VirtualInvokeData _30_get_rectTransform;
            VirtualInvokeData _31_get_defaultMaterial;
            VirtualInvokeData _32_get_material;
            VirtualInvokeData _33_set_material;
            VirtualInvokeData _34_get_materialForRendering;
            VirtualInvokeData _35_get_mainTexture;
            VirtualInvokeData _36_OnCullingChanged;
            VirtualInvokeData _37_Rebuild;
            VirtualInvokeData _38_LayoutComplete;
            VirtualInvokeData _39_GraphicUpdateComplete;
            VirtualInvokeData _40_UpdateMaterial;
            VirtualInvokeData _41_UpdateGeometry;
            VirtualInvokeData _42_OnFillVBO;
            VirtualInvokeData _43_OnPopulateMesh;
            VirtualInvokeData _44_OnPopulateMesh;
            VirtualInvokeData _45_SetNativeSize;
            VirtualInvokeData _46_Raycast;
            VirtualInvokeData _47_CrossFadeColor;
            VirtualInvokeData _48_CrossFadeColor;
            VirtualInvokeData _49_CrossFadeAlpha;
            VirtualInvokeData _50_UnityEngine_UI_IClippable_get_gameObject;
            VirtualInvokeData _51_unknown;
            VirtualInvokeData _52_unknown;
            VirtualInvokeData _53_unknown;
            VirtualInvokeData _54_unknown;
            VirtualInvokeData _55_unknown;
            VirtualInvokeData _56_unknown;
            VirtualInvokeData _57_unknown;
            VirtualInvokeData _58_GetModifiedMaterial;
            VirtualInvokeData _59_Cull;
            VirtualInvokeData _60_SetClipRect;
            VirtualInvokeData _61_SetClipSoftness;
            VirtualInvokeData _62_ParentMaskStateChanged;
            VirtualInvokeData _63_RecalculateClipping;
            VirtualInvokeData _64_RecalculateMasking;
            VirtualInvokeData _65_unknown;
            VirtualInvokeData _66_unknown;
            VirtualInvokeData _67_unknown;
            VirtualInvokeData _68_unknown;
            VirtualInvokeData _69_unknown;
            VirtualInvokeData _70_unknown;
            VirtualInvokeData _71_unknown;
            VirtualInvokeData _72_unknown;
            VirtualInvokeData _73_unknown;
            VirtualInvokeData _74_unknown;
            VirtualInvokeData _75_unknown;
            VirtualInvokeData _76_unknown;
            VirtualInvokeData _77_OnBeforeSerialize;
            VirtualInvokeData _78_OnAfterDeserialize;
            VirtualInvokeData _79_CalculateLayoutInputHorizontal;
            VirtualInvokeData _80_CalculateLayoutInputVertical;
            VirtualInvokeData _81_get_minWidth;
            VirtualInvokeData _82_get_preferredWidth;
            VirtualInvokeData _83_get_flexibleWidth;
            VirtualInvokeData _84_get_minHeight;
            VirtualInvokeData _85_get_preferredHeight;
            VirtualInvokeData _86_get_flexibleHeight;
            VirtualInvokeData _87_get_layoutPriority;
            VirtualInvokeData _88_IsRaycastLocationValid;
        };

        inline void virtual_set_color(UnityEngine::Color::Object value) {
            UnityEngine::Color::Fields proxy = { .r = value.fields.r, .g = value.fields.g, .b = value.fields.b, .a = value.fields.a };
            (*(this->instance()->klass->vtable)._23_set_color.methodPtr)
                    (this->instance(), proxy, this->instance()->klass->vtable._23_set_color.method);
        }

        inline void virtual_set_color(UnityEngine::Color::Fields value) {
            (*(this->instance()->klass->vtable)._23_set_color.methodPtr)
                    (this->instance(), value, this->instance()->klass->vtable._23_set_color.method);
        }

        inline void set_sprite(UnityEngine::Sprite::Object* value) {
            external<void>(0x024b8ed0, this, value);
        }

        inline UnityEngine::Sprite::Object* get_sprite() {
            return external<UnityEngine::Sprite::Object*>(0x024b8ec0, this);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$UnityEngine_UI_Image : List<List$$UnityEngine_UI_Image, UnityEngine::UI::Image> {
        static inline StaticILMethod<0x04c896b8> Method$$ctor {};
    };

    struct Dictionary$$int32_t$$UnityEngine_UI_Image : Dictionary$$int32_t<Dictionary$$int32_t$$UnityEngine_UI_Image, UnityEngine::UI::Image> {
        static inline StaticILMethod<0x04c87bb0> Method$$set_Item {};
        static inline StaticILMethod<0x04c70da8> Method$$get_Values {};
    };

    struct IEnumerable$$UnityEngine_UI_Image : IEnumerable<IEnumerable$$UnityEngine_UI_Image, UnityEngine::UI::Image> {
        static inline StaticILMethod<0x04c71770> Method$$ToList {};
    };
}