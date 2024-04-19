#pragma once

#include "externals/il2cpp.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/System/String.h"
#include "externals/Dpr/UI/UIText.h"

namespace Dpr::UI {
    struct SettingMenuItem : ILClass<SettingMenuItem> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            int32_t _itemType;
            UnityEngine::RectTransform::Object* _contentRoot;
            UnityEngine::UI::Image::Object* _selectBg;
            void* _selectorParam;
            void* _windowSelectorParam;
            void* _gaugeSelectorParam;
            void* _onValueChanged;
            int32_t _configId;
            System::String::Object* _descMessagelabel;
            UnityEngine::RectTransform::Object* _content;
            int32_t _selectIndex;
            System::Collections::Generic::List$$UIText::Object* _texts;
            void* _slider;
            Dpr::UI::UIText::Object* _sliderText;
        };

        inline void ctor() {
            external<void>(0x01d3fed0, this);
        }

        inline void Setup(int32_t configId, int32_t selectIndex, System::String* descMessageLabel, void* onValueChanged) {
            external<void>(0x01d3ee30, this, configId, selectIndex, descMessageLabel, onValueChanged);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$SettingMenuItem : List<List$$SettingMenuItem, Dpr::UI::SettingMenuItem> {
        struct Fields {
            Dpr::UI::SettingMenuItem::Array* _items;
            int32_t _size;
            int32_t _version;
            Il2CppObject* _syncRoot;
        };

        static inline StaticILMethod<0x04c89908> Method$$Add {};

        void insert(Dpr::UI::SettingMenuItem* elem, int32_t pos) {
            this->Add(elem);
            for (int32_t i = this->instance()->fields._size - 1; i > pos; i--) {
                this->swap(i, i - 1);
            }
        }
    };
}
