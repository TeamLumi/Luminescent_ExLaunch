#pragma once
#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/Sprite.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/Dpr/UI/ZukanDescriptionPanel.h"
#include "externals/Dpr/UI/UIMsgWindowController.h"
#include "externals/Dpr/UI/UIWindow.h"

namespace Dpr::UI {
    struct UIZukan : ILClass<UIZukan> {
        enum class ShowZukanType : int32_t {
            None = 0,
            Shinou = 1,
            Zenkoku = 2
        };

        struct __c__DisplayClass66_0_ : ILClass<__c__DisplayClass66_0_, 0x04c632b0> {
            struct Fields {
                int32_t getCount;
                int32_t foundCount;
            };

            static inline StaticILMethod<0x04c80d70> Method$$__SetupZukan__b__0 {};
            static inline StaticILMethod<0x04c80d78> Method$$__SetupZukan__b__1 {};

            inline void ctor() {
                external<void>(0x01a39cf0, this);
            }
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            int32_t _animStateIn;
            int32_t _animStateOut;
            UnityEngine::RectTransform::Object* header;
            UnityEngine::UI::Image::Object* headerImage;
            UnityEngine::Sprite::Array* headerSprites;
            UnityEngine::UI::Image::Object* fadeImage;
            UnityEngine::RectTransform::Object* sliderIconRectTransform;
            void* listScrollView;
            UnityEngine::UI::Image::Object* titleImage;
            Dpr::UI::UIText::Object* getCountText;
            Dpr::UI::UIText::Object* foundCountText;
            Dpr::UI::UIText::Object* sortNameText;
            Dpr::UI::ZukanDescriptionPanel::Object* descriptionPanel;
            UnityEngine::RectTransform::Object* descriptionHideModelViewPosition;
            UnityEngine::RectTransform::Object* descriptionHideInfoButtonPosition;
            UnityEngine::RectTransform::Object* contextMenuPosition;
            Dpr::UI::UIMsgWindowController::Object* msgWindowController;
            System::Collections::Generic::List$$ZukanInfo::Object* zukanInfoList;
            System::Collections::Generic::List$$ZukanInfo::Object* viewZukanInfoList;
            System::Collections::Generic::List$$ZukanInfo::Object* getZukanInfoList;
            void* nameSortOrderDic;
            int32_t buttonViewType;
            void* selectedZukanPokemonInfoButton;
            bool canSwitchZukan;
            bool canShowDescription;
            bool isWaitUpdate;
            bool isShowZenkokuZukan;
            int32_t loadModelCount;
            ShowZukanType showZukanType;
        };

        static_assert(offsetof(Fields, showZukanType) == 0x118);
    };
}