#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Action.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/UnityEngine/EventSystems/UIBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::Battle::View::UI {
    struct BUIButtonBase : ILClass<BUIButtonBase> {
        struct Fields : UnityEngine::EventSystems::UIBehaviour::Fields {
            UnityEngine::UI::Image::Object* _backgroundImage;
            UnityEngine::UI::Image::Object* _foregroundImage;
            TMPro::TextMeshProUGUI::Object* _text;
            int32_t _index;
            int32_t _transition;
            int32_t _state;
            bool _isSelected;
            bool _isEnabele;
            UnityEngine::RectTransform::Object* _cachedRectTransform;
            System::Action::Object* _onSelected;
            System::Action::Object* _onSubmit;
        };

        template <typename T>
        inline bool Submit() {
            return ILClass<T>::template external<bool>(0x021d47f0, this, *T::Method$$Submit);
        }

        template <typename T>
        inline bool Submit(MethodInfo* methodInfo) {
            return ILClass<T>::template external<bool>(0x021d47f0, this, methodInfo);
        }

        // This has a MethodInfo, but it's unused in the method so we just ignore it
        inline void set_Text(System::String::Object* value) {
            external<void>(0x021d4640, this, value);
        }

        // This has a MethodInfo, but it's unused in the method so we just ignore it
        inline UnityEngine::RectTransform::Object* get_rectTransform() {
            return external<UnityEngine::RectTransform::Object*>(0x021d4530, this);
        }
    };
}