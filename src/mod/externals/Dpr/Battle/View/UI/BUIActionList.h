#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BattleViewBase.h"
#include "externals/Dpr/Battle/Logic/BtlAction.h"
#include "externals/Dpr/Battle/View/UI/BattleViewUICanvasBase.h"
#include "externals/Dpr/Battle/View/UI/BUIActionSelectButton.h"
#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
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
            Dpr::Battle::Logic::BtlAction _Result_k__BackingField;
        };

        struct VirtualInvokeData_ForceHide {
            typedef void(*Il2CppMethodPointer)(Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VTable {
            VirtualInvokeData _0_Equals;
            VirtualInvokeData _1_Finalize;
            VirtualInvokeData _2_GetHashCode;
            VirtualInvokeData _3_ToString;
            VirtualInvokeData _4_get_IsValid;
            VirtualInvokeData _5_Startup;
            VirtualInvokeData _6_Reset;
            VirtualInvokeData _7_UnInitialize;
            VirtualInvokeData _8_OnUpdate;
            VirtualInvokeData _9_PreparaNext;
            VirtualInvokeData_ForceHide _10_ForceHide;
            VirtualInvokeData _11_OnShow;
            VirtualInvokeData _12_OnHide;
            VirtualInvokeData _13_OnPlayAnimation;
            VirtualInvokeData _14_SetAlpha;
            VirtualInvokeData _15_unknown;
            VirtualInvokeData _16_get_Result;
        };

        static_assert(offsetof(Fields, _Result_k__BackingField) == 0xA0);

        inline void PreparaNext(bool isForward) {
            external<void>(0x01e8c380, this, isForward);
        }

        inline void UpdateActionButton(bool isPlaySe) {
            external<void>(0x01e7eee0, this, isPlaySe);
        }

        inline void Initialize(Dpr::Battle::Logic::BattleViewBase::SelectActionParam::Object** param) {
            external<void>(0x01e75e20, this, param);
        }

        inline void OnSubmitPokeBall() {
            external<void>(0x01e8c1b0, this);
        }

        inline void virtual_ForceHide() {
            (this->instance()->klass)->vtable->_10_ForceHide.methodPtr(((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)this), (this->instance()->klass)->vtable->_10_ForceHide.method);
        }
    };
}

