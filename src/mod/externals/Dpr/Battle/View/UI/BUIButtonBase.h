#pragma once
#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/EventSystems/UIBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/System/Action.h"



namespace Dpr::Battle::View::UI {
    struct BUIActionSelectButton;

    template <typename T>
    struct BUIButtonBase : ILClass<T> {
        struct Fields : UnityEngine::EventSystems::UIBehaviour::Fields {
            UnityEngine::UI::Image::Object* _backgroundImage;
            UnityEngine::UI::Image::Object* _foregroundImage;
            TMPro::TextMeshProUGUI::Object* _text;
            int32_t index;
            int32_t transition;
            int32_t _state;
            bool _isSelected;
            bool isEnabele;
            UnityEngine::RectTransform::Object* _cachedRectTransform;
            System::Action::Object* _onSelected;
            System::Action::Object* _onSubmit;
        };

        static const inline long BUIActionSelectButton_TypeInfo = 0x04c5dc78;

        inline bool Submit() {
            return ILClass<T>::template external<bool>(0x021d47f0, this, *T::Method$$Submit);
        }
    };
}