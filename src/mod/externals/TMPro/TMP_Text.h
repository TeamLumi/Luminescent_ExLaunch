#pragma once

#include "externals/il2cpp-api.h"
#include "externals/TMPro/VertexGradient.h"
#include "externals/UnityEngine/UI/MaskableGraphic.h"
#include "externals/UnityEngine/Color32.h"

namespace TMPro {
    struct TMP_Text : ILClass<TMP_Text> {
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
            UnityEngine::Color32::Object m_faceColor; //UnityEngine_Color32_o
            UnityEngine::Color32::Object m_outlineColor; //UnityEngine_Color32_o
            float m_outlineWidth;
            float m_fontSize;
            float m_currentFontSize;
            float m_fontSizeBase;
            void* m_sizeStack; //TMPro_TMP_TextProcessingStack_float__o
            int32_t m_fontWeight;
            int32_t m_FontWeightInternal;

            // TODO: Insert missing classes/structs so that the rest below are aligned.
            void* m_FontWeightStack; //TMPro_TMP_TextProcessingStack_FontWeight__o
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
            void* m_fontStyleStack; //TMPro_TMP_FontStyleStack_o
            bool m_isUsingBold;
            int32_t m_HorizontalAlignment;
            int32_t m_VerticalAlignment;
            int32_t m_textAlignment;
            int32_t m_lineJustification;
            void* m_lineJustificationStack; //TMPro_TMP_TextProcessingStack_HorizontalAlignmentOptions__o
            void* m_textContainerLocalCorners; //UnityEngine_Vector3_array*
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
            void* m_linkedTextComponent; //TMPro_TMP_Text_o*
            void* parentLinkedComponent; //TMPro_TMP_Text_o*
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
            void* m_margin; //UnityEngine_Vector4_o
            float m_marginLeft;
            float m_marginRight;
            float m_marginWidth;
            float m_marginHeight;
            float m_width;
            void* m_textInfo; //TMPro_TMP_TextInfo_o*
            bool m_havePropertiesChanged;
            bool m_isUsingLegacyAnimationComponent;
            void* m_transform; //UnityEngine_Transform_o*
            void* m_rectTransform; //UnityEngine_RectTransform_o*
            void* m_PreviousRectTransformSize; //UnityEngine_Vector2_o
            void* m_PreviousPivotPosition; //UnityEngine_Vector2_o
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
            void* m_indentStack; //TMPro_TMP_TextProcessingStack_float__o
            bool tag_NoParsing;
            bool m_isParsingText;
            void* m_FXMatrix; //UnityEngine_Matrix4x4_o
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
            void* m_meshExtents; //TMPro_Extents_o
            UnityEngine::Color32::Object m_htmlColor;
            void* m_colorStack; //TMPro_TMP_TextProcessingStack_Color32__o
            void* m_underlineColorStack; //TMPro_TMP_TextProcessingStack_Color32__o
            void* m_strikethroughColorStack; //TMPro_TMP_TextProcessingStack_Color32__o
            void* m_HighlightStateStack; //TMPro_TMP_TextProcessingStack_HighlightState__o
            void* m_colorGradientPreset; //TMPro_TMP_ColorGradient_o*
            void* m_colorGradientStack; //TMPro_TMP_TextProcessingStack_TMP_ColorGradient__o
            bool m_colorGradientPresetIsTinted;
            float m_tabSpacing;
            float m_spacing;
            void* m_TextStyleStacks; //TMPro_TMP_TextProcessingStack_int__array*
            int32_t m_TextStyleStackDepth;
            void* m_ItalicAngleStack; //TMPro_TMP_TextProcessingStack_int__o
            int32_t m_ItalicAngle;
            void* m_actionStack; //TMPro_TMP_TextProcessingStack_int__o
            float m_padding;
            float m_baselineOffset;
            void* m_baselineOffsetStack; //TMPro_TMP_TextProcessingStack_float__o
            float m_xAdvance;
            int32_t m_textElementType;
            void* m_cached_TextElement; //TMPro_TMP_TextElement_o*
            void* m_Ellipsis; //TMPro_TMP_Text_SpecialCharacter_o
            void* m_Underline; //TMPro_TMP_Text_SpecialCharacter_o
            void* m_defaultSpriteAsset; //TMPro_TMP_SpriteAsset_o*
            void* m_currentSpriteAsset; //TMPro_TMP_SpriteAsset_o*
            int32_t m_spriteCount;
            int32_t m_spriteIndex;
            int32_t m_spriteAnimationID;
            bool m_ignoreActiveState;
            void* m_TextBackingArray; //TMPro_TMP_Text_TextBackingContainer_o
            void* k_Power; //System_Decimal_array*
        };

        inline void set_text(System::String::Object* value) {
            external<void>(0x01e94520, this, value);
        }

        inline void set_color(UnityEngine::Color::Object value) {
            UnityEngine::Color::Fields valueProxy = { .r = value.fields.r, .g = value.fields.g, .b = value.fields.b, .a = value.fields.a };
            external<void>(0x01e94aa0, this, valueProxy);
        }
    };
}