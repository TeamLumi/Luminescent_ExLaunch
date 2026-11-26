#pragma once

#include "externals/il2cpp.h"

#include "externals/Dpr/UI/UIText.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/UI/Slider.h"

namespace Dpr::UI {
    struct SettingMenuItem : ILClass<SettingMenuItem> {
        struct __DisplayClass32_0 : ILClass<__DisplayClass32_0, game::Address::Dpr_UI_SettingMenuItem_c_DisplayClass32_0_TypeInfo> {
            struct Fields {
                int32_t selectIndex;
            };

            static inline StaticILMethod<game::Address::Method_Dpr_UI_SettingMenuItem_c_DisplayClass32_0_SetSelectIndex_b_0> Method$$SetSelectIndex_b0 {};

            inline void ctor() {
                external<void>(game::Address::Dpr_UI_SettingMenuItem_c_DisplayClass32_0_ctor, this);
            }
        };

        struct SelectorParam : ILClass<SelectorParam> {
            struct Fields {
                UnityEngine::Color::Array* textColors;
            };
        };

        struct WindowSelectorParam : ILClass<WindowSelectorParam> {
            struct Fields {
                UnityEngine::UI::Image::Array* arrows;
                UnityEngine::Color::Array* textColors;
                UnityEngine::Color::Array* arrowColors;
            };
        };

        struct GaugeSelectorParam : ILClass<GaugeSelectorParam> {
            struct Fields {
                UnityEngine::UI::Image::Object* gaugeBg;
                UnityEngine::UI::Image::Object* gauge;
                UnityEngine::UI::Image::Object* handle;
                UnityEngine::Color::Array* gaugeBgColors;
                UnityEngine::Color::Array* gaugeColors;
                UnityEngine::Sprite::Array* spriteHandles;
                UnityEngine::Color::Array* textColors;
            };
        };

        enum class ItemType : int32_t {
            Selector = 0,
            Gauge = 1,
            WindowSelector = 2,
        };

        struct Fields : UnityEngine::MonoBehaviour::Fields {
            ItemType _itemType;
            UnityEngine::RectTransform::Object* _contentRoot;
            UnityEngine::UI::Image::Object* _selectBg;
            SelectorParam::Object* _selectorParam;
            WindowSelectorParam::Object* _windowSelectorParam;
            GaugeSelectorParam::Object* _gaugeSelectorParam;
            UnityEngine::Events::UnityAction::Object* _onValueChanged;
            int32_t _configId;
            System::String::Object* _descMessagelabel;
            UnityEngine::RectTransform::Object* _content;
            int32_t _selectIndex;
            System::Collections::Generic::List$$UIText::Object* _texts;
            UnityEngine::UI::Slider::Object* _slider;
            Dpr::UI::UIText::Object* _sliderText;
        };

        inline void ctor() {
            external<void>(game::Address::Dpr_UI_SettingMenuItem_ctor, this);
        }

        inline void Setup(int32_t configId, int32_t selectIndex, System::String* descMessageLabel, UnityEngine::Events::UnityAction::Object* onValueChanged) {
            external<void>(game::Address::Dpr_UI_SettingMenuItem_Setup, this, configId, selectIndex, descMessageLabel, onValueChanged);
        }

        inline bool SetSelectIndex(int32_t selectIndex, bool isEqualChecked, bool isCallbacked) {
            return external<bool>(game::Address::Dpr_UI_SettingMenuItem_SetSelectIndex, this, selectIndex, isEqualChecked, isCallbacked);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$SettingMenuItem : List<List$$SettingMenuItem, Dpr::UI::SettingMenuItem> {

        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_SettingMenuItem_Add> Method$$Add {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_SettingMenuItem_Clear> Method$$Clear {};

        void insert(Dpr::UI::SettingMenuItem* elem, int32_t pos) {
            this->Add(elem);
            for (int32_t i = this->instance()->fields._size - 1; i > pos; i--) {
                this->swap(i, i - 1);
            }
        }
    };
}
