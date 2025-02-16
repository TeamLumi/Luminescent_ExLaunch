#pragma once

#include "externals/il2cpp-api.h"
#include "externals/TMPro/TMP_Text.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/Rect.h"

namespace TMPro {
    struct TextMeshProUGUI : ILClass<TextMeshProUGUI, 0x04c59018> {
        struct Fields : TMPro::TMP_Text::Fields {
            bool m_hasFontAssetChanged;
            void* m_subTextObjects; //TMPro_TMP_SubMeshUI_array*
            float m_previousLossyScaleY;
            void* m_RectTransformCorners; //UnityEngine_Vector3_array*
            void* m_canvasRenderer; //UnityEngine_CanvasRenderer_o*
            void* m_canvas; //UnityEngine_Canvas_o*
            bool m_isFirstAllocation;
            int32_t m_max_characters;
            void* m_baseMaterial; //UnityEngine_Material_o*
            bool m_isScrollRegionSet;
            UnityEngine::Vector4::Object m_maskOffset;
            UnityEngine::Matrix4x4::Object m_EnvMapMatrix; //UnityEngine_Matrix4x4_o
            bool m_isRegisteredForEvents;
            bool m_isRebuildingLayout;
            UnityEngine::Coroutine::Object* m_DelayedGraphicRebuild;
            UnityEngine::Coroutine::Object* m_DelayedMaterialRebuild;
            UnityEngine::Rect::Object m_ClipRect;
            bool m_ValidRect;
            void* OnPreRenderText; //System_Action_TMP_TextInfo__o*
        };

        static_assert(offsetof(Fields, OnPreRenderText) == 0x760);

        struct VirtualInvokeData_get_color {
            typedef UnityEngine::Color::Fields(*Il2CppMethodPointer)(TMPro::TextMeshProUGUI::Object* __this, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VirtualInvokeData_set_color {
            typedef void(*Il2CppMethodPointer)(TMPro::TextMeshProUGUI::Object* __this, UnityEngine::Color::Fields value, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VirtualInvokeData_set_text {
            typedef void(*Il2CppMethodPointer)(TMPro::TextMeshProUGUI::Object* __this, System::String::Object* value, const MethodInfo*);
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
            VirtualInvokeData_get_color _22_get_color;
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
            VirtualInvokeData _65_get_text;
            VirtualInvokeData_set_text _66_set_text;
            VirtualInvokeData _67_get_fontSharedMaterial;
            VirtualInvokeData _68_set_fontSharedMaterial;
            VirtualInvokeData _69_get_fontSharedMaterials;
            VirtualInvokeData _70_set_fontSharedMaterials;
            VirtualInvokeData _71_get_fontMaterials;
            VirtualInvokeData _72_set_fontMaterials;
            VirtualInvokeData _73_get_margin;
            VirtualInvokeData _74_set_margin;
            VirtualInvokeData _75_get_autoSizeTextContainer;
            VirtualInvokeData _76_set_autoSizeTextContainer;
            VirtualInvokeData _77_get_mesh;
            VirtualInvokeData _78_add_OnPreRenderText;
            VirtualInvokeData _79_remove_OnPreRenderText;
            VirtualInvokeData _80_get_flexibleHeight;
            VirtualInvokeData _81_get_flexibleWidth;
            VirtualInvokeData _82_get_minWidth;
            VirtualInvokeData _83_get_minHeight;
            VirtualInvokeData _84_get_preferredWidth;
            VirtualInvokeData _85_get_preferredHeight;
            VirtualInvokeData _86_get_renderedWidth;
            VirtualInvokeData _87_get_renderedHeight;
            VirtualInvokeData _88_get_layoutPriority;
            VirtualInvokeData _89_LoadFontAsset;
            VirtualInvokeData _90_SetSharedMaterial;
            VirtualInvokeData _91_GetMaterial;
            VirtualInvokeData _92_SetFontBaseMaterial;
            VirtualInvokeData _93_GetSharedMaterials;
            VirtualInvokeData _94_SetSharedMaterials;
            VirtualInvokeData _95_GetMaterials;
            VirtualInvokeData _96_CreateMaterialInstance;
            VirtualInvokeData _97_SetFaceColor;
            VirtualInvokeData _98_SetOutlineColor;
            VirtualInvokeData _99_SetOutlineThickness;
            VirtualInvokeData _100_SetShaderDepth;
            VirtualInvokeData _101_SetCulling;
            VirtualInvokeData _102_UpdateCulling;
            VirtualInvokeData _103_GetPaddingForMaterial;
            VirtualInvokeData _104_GetPaddingForMaterial;
            VirtualInvokeData _105_GetTextContainerLocalCorners;
            VirtualInvokeData _106_ForceMeshUpdate;
            VirtualInvokeData _107_UpdateGeometry;
            VirtualInvokeData _108_UpdateVertexData;
            VirtualInvokeData _109_UpdateVertexData;
            VirtualInvokeData _110_SetVertices;
            VirtualInvokeData _111_UpdateMeshPadding;
            VirtualInvokeData _112_InternalCrossFadeColor;
            VirtualInvokeData _113_InternalCrossFadeAlpha;
            VirtualInvokeData _114_SetArraySizes;
            VirtualInvokeData _115_CalculatePreferredValues;
            VirtualInvokeData _116_GetCompoundBounds;
            VirtualInvokeData _117_GetCanvasSpaceClippingRect;
            VirtualInvokeData _118_GetTextInfo;
            VirtualInvokeData _119_ComputeMarginSize;
            VirtualInvokeData _120_SaveGlyphVertexInfo;
            VirtualInvokeData _121_SaveSpriteVertexInfo;
            VirtualInvokeData _122_FillCharacterVertexBuffers;
            VirtualInvokeData _123_FillCharacterVertexBuffers;
            VirtualInvokeData _124_FillSpriteVertexBuffers;
            VirtualInvokeData _125_DrawUnderlineMesh;
            VirtualInvokeData _126_DrawTextHighlight;
            VirtualInvokeData _127_SetActiveSubMeshes;
            VirtualInvokeData _128_DestroySubMeshObjects;
            VirtualInvokeData _129_ClearMesh;
            VirtualInvokeData _130_ClearMesh;
            VirtualInvokeData _131_GetParsedText;
            VirtualInvokeData _132_InternalUpdate;
            VirtualInvokeData _133_CalculateLayoutInputHorizontal;
            VirtualInvokeData _134_CalculateLayoutInputVertical;
            VirtualInvokeData _135_unknown;
            VirtualInvokeData _136_unknown;
            VirtualInvokeData _137_unknown;
            VirtualInvokeData _138_unknown;
            VirtualInvokeData _139_unknown;
            VirtualInvokeData _140_unknown;
            VirtualInvokeData _141_unknown;
            VirtualInvokeData _142_GenerateTextMesh;
        };

        inline UnityEngine::Color::Object virtual_get_color() {
            return {
                .fields = (*(this->instance()->klass->vtable)._22_get_color.methodPtr)
                            (this->instance(), this->instance()->klass->vtable._22_get_color.method)
            };
        }

        inline void virtual_set_color(UnityEngine::Color::Object value) {
            UnityEngine::Color::Fields proxy = { .r = value.fields.r, .g = value.fields.g, .b = value.fields.b, .a = value.fields.a };
            (*(this->instance()->klass->vtable)._23_set_color.methodPtr)
                    (this->instance(), proxy, this->instance()->klass->vtable._23_set_color.method);
        }

        inline void virtual_set_color(UnityEngine::Color::Fields value) {
            (*(this->instance()->klass->vtable)._23_set_color.methodPtr)
                    (this->instance(), value, this->instance()->klass->vtable._23_set_color.method);
        }

        inline void virtual_set_text(System::String::Object* value) {
            (*(this->instance()->klass->vtable)._66_set_text.methodPtr)
                    (this->instance(), value, this->instance()->klass->vtable._66_set_text.method);
        }
    };
}
