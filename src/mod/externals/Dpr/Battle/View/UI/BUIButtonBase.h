#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/Action.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/EventSystems/UIBehaviour.h"

namespace Dpr::Battle::View::UI {
    struct BUIActionSelectButton;

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

        // The MethodInfo in this method is unused so not necessary
        inline bool Submit() {
            return external<bool>(0x021d47f0, this);
        }

        // The MethodInfo in this method is unused so not necessary
        inline UnityEngine::RectTransform::Object* get_rectTransform() {
            return external<UnityEngine::RectTransform::Object*>(0x021d4530, this);
        }
    };
}
