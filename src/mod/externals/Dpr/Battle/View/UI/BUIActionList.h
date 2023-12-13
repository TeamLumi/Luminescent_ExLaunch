#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/View/UI/BattleViewUICanvasBase.h"
#include "externals/Dpr/Battle/View/UI/BUIActionSelectButton.h"
#include "externals/UnityEngine/GameObject.h"

namespace Dpr::Battle::View::UI {
    struct BUIActionList : ILClass<BUIActionList> {
        struct Fields : Dpr::Battle::View::UI::BattleViewUICanvasBase::Fields {
            UnityEngine::GameObject::Object* _xyButtonFrame;
            UnityEngine::GameObject::Object* _yButtonFrame;
            void* _pokeBallButton;
            void* _situationButton;
            void* _buttonLayout;
            System::Collections::Generic::List$$BUIActionSelectButton::Object* _actionButtons;
            UnityEngine::UI::Image::Object* _xyMenuBallIcon;
            bool _isBallEnable;
            bool _needOpenBallWindow;
            bool _isSafari;
            bool isButtonAction;
            int32_t _minIndex;
            int32_t _maxIndex;
            bool _IsReturnable_k__BackingField;
            int32_t _Result_k__BackingField;
        };

        static_assert(sizeof(Fields) == 0xa8);
        static_assert(offsetof(Fields, _isBallEnable) == 0x90);
        static_assert(offsetof(Fields, isButtonAction) == 0x93);

        inline void OnSubmitPokeBall() {
            external<void>(0x01e8c1b0, this);
        }

        inline void PreparaNext(bool isForward) {
            external<void>(0x01e8c380, this, isForward);
        }
    };
}