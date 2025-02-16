#pragma once

#include "externals/il2cpp-api.h"

#include "externals/TMPro/Extents.h"
#include "externals/TMPro/TMP_ColorGradient.h"
#include "externals/TMPro/TMP_FontStyleStack.h"
#include "externals/TMPro/TMP_TextInfo.h"
#include "externals/TMPro/TMP_TextProcessingStack.h"
#include "externals/TMPro/VertexGradient.h"
#include "externals/UnityEngine/Color32.h"
#include "externals/UnityEngine/Material.h"
#include "externals/UnityEngine/Matrix4x4.h"
#include "externals/UnityEngine/UI/MaskableGraphic.h"
#include "externals/UnityEngine/Vector4.h"

namespace TMPro {
    struct TMP_Text : ILClass<TMP_Text> {
        struct SpecialCharacter : ILStruct<SpecialCharacter> {
            struct Fields {
                void* character;
                void* fontAsset;
                UnityEngine::Material::Object* material;
                int32_t materialIndex;
            };
        };

        struct TextBackingContainer : ILStruct<TextBackingContainer> {
            struct Fields {
                System::UInt32_array* m_Array;
                int32_t m_Count;
            };
        };

        struct Fields : UnityEngine::UI::MaskableGraphic::Fields {
            System::String::Object* m_text;
            bool m_IsTextBackingStringDirty;
            void* m_TextPreprocessor; //TMPro_ITextPreprocessor_o*
            bool m_isRightToLeft;
            void* m_fontAsset; //TMPro_TMP_FontAsset_o*
            void* m_currentFontAsset; //TMPro_TMP_FontAsset_o*
            bool m_isSDFShader;
            void* m_sharedMaterial; //UnityEngine_Material_o*
            void* m_currentMaterial; //UnityEngine_Material_o*
            int32_t m_currentMaterialIndex;
            void* m_fontSharedMaterials; //UnityEngine_Material_array*
            void* m_fontMaterial; //UnityEngine_Material_o*
            void* m_fontMaterials; //UnityEngine_Material_array*
            bool m_isMaterialDirty;
            UnityEngine::Color32::Object m_fontColor32;
            UnityEngine::Color::Object m_fontColor;
            UnityEngine::Color32::Object m_underlineColor;
            UnityEngine::Color32::Object m_strikethroughColor;
            bool m_enableVertexGradient;
            int32_t m_colorMode;
            TMPro::VertexGradient::Object m_fontColorGradient;
            void* m_fontColorGradientPreset; //TMPro_TMP_ColorGradient_o*
            void* m_spriteAsset; //TMPro_TMP_SpriteAsset_o*
            bool m_tintAllSprites;
            bool m_tintSprite;
            UnityEngine::Color32::Object m_spriteColor;
            void* m_StyleSheet; //TMPro_TMP_StyleSheet_o*
            void* m_TextStyle; //TMPro_TMP_Style_o*
            int32_t m_TextStyleHashCode;
            bool m_overrideHtmlColors;
            UnityEngine::Color32::Object m_faceColor;
            UnityEngine::Color32::Object m_outlineColor;
            float m_outlineWidth;
            float m_fontSize;
            float m_currentFontSize;
            float m_fontSizeBase;
            TMP_TextProcessingStack_float::Object m_sizeStack;
            int32_t m_fontWeight;
            int32_t m_FontWeightInternal;
            TMP_TextProcessingStack_int32_t::Object m_FontWeightStack;
            bool m_enableAutoSizing;
            float m_maxFontSize;
            float m_minFontSize;
            int32_t m_AutoSizeIterationCount;
            int32_t m_AutoSizeMaxIterationCount;
            bool m_IsAutoSizePointSizeSet;
            float m_fontSizeMin;
            float m_fontSizeMax;
            int32_t m_fontStyle;
            int32_t m_FontStyleInternal;
            TMP_FontStyleStack::Object m_fontStyleStack;
            bool m_isUsingBold;
            int32_t m_HorizontalAlignment;
            int32_t m_VerticalAlignment;
            int32_t m_textAlignment;
            int32_t m_lineJustification;
            TMP_TextProcessingStack_int32_t::Object m_lineJustificationStack;
            UnityEngine::Vector3::Array* m_textContainerLocalCorners;
            float m_characterSpacing;
            float m_cSpacing;
            float m_monoSpacing;
            float m_wordSpacing;
            float m_lineSpacing;
            float m_lineSpacingDelta;
            float m_lineHeight;
            bool m_IsDrivenLineSpacing;
            float m_lineSpacingMax;
            float m_paragraphSpacing;
            float m_charWidthMaxAdj;
            float m_charWidthAdjDelta;
            bool m_enableWordWrapping;
            bool m_isCharacterWrappingEnabled;
            bool m_isNonBreakingSpace;
            bool m_isIgnoringAlignment;
            float m_wordWrappingRatios;
            int32_t m_overflowMode;
            int32_t m_firstOverflowCharacterIndex;
            TMP_Text::Object* m_linkedTextComponent;
            TMP_Text::Object* parentLinkedComponent;
            bool m_isTextTruncated;
            bool m_enableKerning;
            float m_GlyphHorizontalAdvanceAdjustment;
            bool m_enableExtraPadding;
            bool checkPaddingRequired;
            bool m_isRichText;
            bool m_parseCtrlCharacters;
            bool m_isOverlay;
            bool m_isOrthographic;
            bool m_isCullingEnabled;
            bool m_isMaskingEnabled;
            bool isMaskUpdateRequired;
            bool m_ignoreCulling;
            int32_t m_horizontalMapping;
            int32_t m_verticalMapping;
            float m_uvLineOffset;
            int32_t m_renderMode;
            int32_t m_geometrySortingOrder;
            bool m_IsTextObjectScaleStatic;
            bool m_VertexBufferAutoSizeReduction;
            int32_t m_firstVisibleCharacter;
            int32_t m_maxVisibleCharacters;
            int32_t m_maxVisibleWords;
            int32_t m_maxVisibleLines;
            bool m_useMaxVisibleDescender;
            int32_t m_pageToDisplay;
            bool m_isNewPage;
            UnityEngine::Vector4::Object m_margin;
            float m_marginLeft;
            float m_marginRight;
            float m_marginWidth;
            float m_marginHeight;
            float m_width;
            TMP_TextInfo::Object* m_textInfo;
            bool m_havePropertiesChanged;
            bool m_isUsingLegacyAnimationComponent;
            void* m_transform; //UnityEngine_Transform_o*
            void* m_rectTransform; //UnityEngine_RectTransform_o*
            UnityEngine::Vector2::Object m_PreviousRectTransformSize;
            UnityEngine::Vector2::Object m_PreviousPivotPosition;
            bool _autoSizeTextContainer_k__BackingField;
            bool m_autoSizeTextContainer;
            void* m_mesh; //UnityEngine_Mesh_o*
            bool m_isVolumetricText;
            void* OnPreRenderText; //System_Action_TMP_TextInfo__o*
            void* m_spriteAnimator; //TMPro_TMP_SpriteAnimator_o*
            float m_flexibleHeight;
            float m_flexibleWidth;
            float m_minWidth;
            float m_minHeight;
            float m_maxWidth;
            float m_maxHeight;
            void* m_LayoutElement; //UnityEngine_UI_LayoutElement_o*
            float m_preferredWidth;
            float m_renderedWidth;
            bool m_isPreferredWidthDirty;
            float m_preferredHeight;
            float m_renderedHeight;
            bool m_isPreferredHeightDirty;
            bool m_isCalculatingPreferredValues;
            int32_t m_layoutPriority;
            bool m_isLayoutDirty;
            bool m_isAwake;
            bool m_isWaitingOnResourceLoad;
            int32_t m_inputSource;
            float m_fontScaleMultiplier;
            float tag_LineIndent;
            float tag_Indent;
            TMP_TextProcessingStack_float::Object m_indentStack;
            bool tag_NoParsing;
            bool m_isParsingText;
            UnityEngine::Matrix4x4::Object m_FXMatrix;
            bool m_isFXMatrixSet;
            void* m_TextProcessingArray; //TMPro_TMP_Text_UnicodeChar_array*
            int32_t m_InternalTextProcessingArraySize;
            void* m_internalCharacterInfo; //TMPro_TMP_CharacterInfo_array*
            int32_t m_totalCharacterCount;
            int32_t m_characterCount;
            int32_t m_firstCharacterOfLine;
            int32_t m_firstVisibleCharacterOfLine;
            int32_t m_lastCharacterOfLine;
            int32_t m_lastVisibleCharacterOfLine;
            int32_t m_lineNumber;
            int32_t m_lineVisibleCharacterCount;
            int32_t m_pageNumber;
            float m_PageAscender;
            float m_maxTextAscender;
            float m_maxCapHeight;
            float m_ElementAscender;
            float m_ElementDescender;
            float m_maxLineAscender;
            float m_maxLineDescender;
            float m_startOfLineAscender;
            float m_startOfLineDescender;
            float m_lineOffset;
            Extents::Object m_meshExtents;
            UnityEngine::Color32::Object m_htmlColor;
            TMP_TextProcessingStack_Color32::Object m_colorStack;
            TMP_TextProcessingStack_Color32::Object m_underlineColorStack;
            TMP_TextProcessingStack_Color32::Object m_strikethroughColorStack;
            TMP_TextProcessingStack_HighlightState::Object m_HighlightStateStack;
            TMP_ColorGradient::Object* m_colorGradientPreset;
            TMP_TextProcessingStack_TMP_ColorGradient::Object m_colorGradientStack;
            bool m_colorGradientPresetIsTinted;
            float m_tabSpacing;
            float m_spacing;
            void* m_TextStyleStacks; //TMPro_TMP_TextProcessingStack_int__array*
            int32_t m_TextStyleStackDepth;
            TMP_TextProcessingStack_int32_t::Object m_ItalicAngleStack;
            int32_t m_ItalicAngle;
            TMP_TextProcessingStack_int32_t::Object m_actionStack;
            float m_padding;
            float m_baselineOffset;
            TMP_TextProcessingStack_float::Object m_baselineOffsetStack;
            float m_xAdvance;
            int32_t m_textElementType;
            void* m_cached_TextElement; //TMPro_TMP_TextElement_o*
            SpecialCharacter::Object m_Ellipsis;
            SpecialCharacter::Object m_Underline;
            void* m_defaultSpriteAsset; //TMPro_TMP_SpriteAsset_o*
            void* m_currentSpriteAsset; //TMPro_TMP_SpriteAsset_o*
            int32_t m_spriteCount;
            int32_t m_spriteIndex;
            int32_t m_spriteAnimationID;
            bool m_ignoreActiveState;
            TextBackingContainer::Object m_TextBackingArray; //TMPro_TMP_Text_TextBackingContainer_o
            void* k_Power; //System_Decimal_array*
        };

        static_assert(offsetof(Fields, tag_NoParsing) == 0x408);
        static_assert(offsetof(Fields, m_isParsingText) == 0x409);
        static_assert(offsetof(Fields, m_FXMatrix) == 0x40c);
        static_assert(offsetof(Fields, m_totalCharacterCount) == 0x468);
        static_assert(offsetof(Fields, k_Power) == 0x698);

        inline void set_text(System::String::Object* value) {
            external<void>(0x01e94520, this, value);
        }

        inline void set_color(UnityEngine::Color::Object value) {
            UnityEngine::Color::Fields valueProxy = { .r = value.fields.r, .g = value.fields.g, .b = value.fields.b, .a = value.fields.a };
            external<void>(0x01e94aa0, this, valueProxy);
        }
    };
}