#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/UI/MaskableGraphic.h"
#include "externals/System/String.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/System/Collections/Generic/List.h"

namespace Dpr::UI {
    struct UIText : ILClass<UIText> {
        struct Fields : TMPro::TextMeshProUGUI::Fields {
            // TODO: The class this inherits from has misaligned fields because of missing classes/structs. Do not use the below fields until this is fixed.
            int32_t _sizeId;
            bool _useMessage;
            System::String::Object* _messageFile;
            System::String::Object* _messageId;
            bool _useTag;
            bool _isManual;
            int32_t _fontMaterialDataIndex;
            bool _uiTextEnable;
            void* _msgFile;
            int32_t _messageIndex;
        };

        struct VirtualInvokeData_set_color {
            typedef void(*Il2CppMethodPointer)(UIText::Object*, UnityEngine::Color::Fields, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VirtualInvokeData_set_text {
            typedef void(*Il2CppMethodPointer)(UIText::Object*, System::String::Object*, const MethodInfo*);
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

        static_assert(offsetof(VTable, _66_set_text) == 0x420);

        inline void SetupMessage(System::String::Object* messageFile, System::String::Object* messageId) {
            external<void>(0x01dd18d0, this, messageFile, messageId);
        }

        inline void SetupMessage(System::String::Object* messageFile, int32_t messageIndex) {
            external<void>(0x01dd1940, this, messageFile, messageIndex);
        }

        inline void ChangeLanguage(int32_t langId) {
            external<void>(0x01dd19b0, this, langId);
        }

        inline void virtual_set_color(UnityEngine::Color::Object value) {
            UnityEngine::Color::Fields valueProxy = { .r = value.fields.r, .g = value.fields.g, .b = value.fields.b, .a = value.fields.a };
            (this->instance()->klass)->vtable._23_set_color.methodPtr(((UIText::Object*)this), valueProxy, (this->instance()->klass)->vtable._23_set_color.method);
        }

        inline void virtual_set_text(System::String::Object* value) {
            (this->instance()->klass)->vtable._66_set_text.methodPtr(((UIText::Object*)this), value, (this->instance()->klass)->vtable._66_set_text.method);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$UIText : List<List$$UIText, Dpr::UI::UIText> {

        static inline StaticILMethod<0x04c89ac0, Dpr::UI::UIText> Method$$UIText$$Add {};
        static inline StaticILMethod<0x04c89ac8, Dpr::UI::UIText> Method$$UIText$$Clear {};
    };

}
