#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/System/Action.h"

namespace Dpr::Battle::View::UI {
    struct BUIActionSelectButton;
    struct BUISituationButton;
    struct BUISituationDescriptionButton;
    struct BUIWazaButton;

    struct BattleViewUICanvasBase : ILClass<BattleViewUICanvasBase> {
        struct TransitionParams : ILStruct<TransitionParams> {
            struct Fields {
                UnityEngine::Vector2::Object HideAnchorPosition;
                UnityEngine::Vector2::Object ShowAnchorPosition;
                int32_t Ease;
                float Duration;
                float Delay;
            };
        };

        struct Fields : public UnityEngine::MonoBehaviour::Fields {
            uint8_t _transitionType;
            TransitionParams::Object _transitionParams;
            UnityEngine::RectTransform::Object* _cachedRectTransform;
            void* _canvasGroup;
            System::Action::Object* _onShowComplete;
            System::Action::Object* _onHideComplete;
            int32_t _MaxIndex_k__BackingField;
            int32_t _CurrentIndex_k__BackingField;
            bool _IsFocus_k__BackingField;
            bool _IsShow_k__BackingField;
            bool _IsValid_k__BackingField;
            bool _IsTransition_k__BackingField;
            int32_t _animationState_k__BackingField;
        };

        static inline StaticILMethod<game::Address::Method_Dpr_Battle_View_UI_BattleViewUICanvasBase_SelectButton_BUISituationButton, BUISituationButton> Method$$BUISituationButton$$SelectButton {};
        static inline StaticILMethod<game::Address::Method_Dpr_Battle_View_UI_BattleViewUICanvasBase_SelectButton_BUISituationDescriptionButton, BUISituationDescriptionButton> Method$$BUISituationDescriptionButton$$SelectButton {};
        static inline StaticILMethod<game::Address::Method_Dpr_Battle_View_UI_BattleViewUICanvasBase_SelectButton_BUIWazaButton, BUIWazaButton> Method$$BUIWazaButton$$SelectButton {};

        template <typename T>
        inline void SelectButton(void* buttons, int32_t index, bool isPlaySe, ILMethod<T>& method) {
            external<void>(game::Address::Dpr_Battle_View_UI_BattleViewUICanvasBase_SelectButton_BUIActionSelectButton, this, buttons, index, isPlaySe, *method);
        }

        inline void Show(System::Action::Object* onComplete) {
            external<void>(game::Address::Dpr_Battle_View_UI_BattleViewUICanvasBase_Show, this, onComplete);
        }

        inline void Hide(bool isForce, System::Action::Object* onComplete) {
            external<void>(game::Address::Dpr_Battle_View_UI_BattleViewUICanvasBase_Hide, this, isForce, onComplete);
        }

        inline void OnShow() {
            external<void>(game::Address::Dpr_Battle_View_UI_BattleViewUICanvasBase_OnShow, this);
        }

        inline void PlayTransitionAnimation(bool isShow) {
            external<void>(game::Address::Dpr_Battle_View_UI_BattleViewUICanvasBase_PlayTransitionAnimation, this, isShow);
        }
    };
}
