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
        struct __DisplayClass32_0 : ILClass<__DisplayClass32_0, 0x04c623f8> {
            struct Fields {
                int32_t selectIndex;
            };

            static inline StaticILMethod<0x04c7fbd0> Method$$SetSelectIndex_b0 {};

            inline void ctor() {
                external<void>(0x01d3fec0, this);
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
            external<void>(0x01d3fed0, this);
        }

        inline void Setup(int32_t configId, int32_t selectIndex, System::String* descMessageLabel, UnityEngine::Events::UnityAction::Object* onValueChanged) {
            external<void>(0x01d3ee30, this, configId, selectIndex, descMessageLabel, onValueChanged);
        }

        inline bool SetSelectIndex(int32_t selectIndex, bool isEqualChecked, bool isCallbacked) {
            return external<bool>(0x01d3f1a0, this, selectIndex, isEqualChecked, isCallbacked);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$SettingMenuItem : List<List$$SettingMenuItem, Dpr::UI::SettingMenuItem> {

        static inline StaticILMethod<0x04c89908> Method$$Add {};
        static inline StaticILMethod<0x04c89910> Method$$Clear {};

        void insert(Dpr::UI::SettingMenuItem* elem, int32_t pos) {
            this->Add(elem);
            for (int32_t i = this->instance()->fields._size - 1; i > pos; i--) {
                this->swap(i, i - 1);
            }
        }
    };
}
