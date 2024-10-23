#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/View/UI/BattleViewUICanvasBase.h"
#include "externals/Dpr/Battle/View/UI/BUISituationButton.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::Battle::View::UI {
    struct BUISituation : ILClass<BUISituation> {
        struct Fields : Dpr::Battle::View::UI::BattleViewUICanvasBase::Fields {
            BUISituationButton::Array* _situationButtons;
            System::Collections::Generic::Dictionary$$int32_t$$BTL_POKEPARAM::Object* _monsParams;
            System::Collections::Generic::Dictionary$$int32_t$$String::Object* _trainerNames;
            System::Collections::Generic::Dictionary$$int32_t$$UnityEngine_UI_Image::Object* _icons;
        };

        static_assert(offsetof(Fields, _icons) == 0x70);

        struct VirtualInvokeData_OnUpdate {
            typedef void(*Il2CppMethodPointer)(Dpr::Battle::View::UI::BUISituation::Object* __this, float deltaTime, const MethodInfo*);
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
            VirtualInvokeData_OnUpdate _8_OnUpdate;
            VirtualInvokeData _9_PreparaNext;
            VirtualInvokeData _10_ForceHide;
            VirtualInvokeData _11_OnShow;
            VirtualInvokeData _12_OnHide;
            VirtualInvokeData _13_OnPlayAnimation;
            VirtualInvokeData _14_SetAlpha;
        };

        inline void virtual_OnUpdate(float deltaTime) {
            (*(this->instance()->klass->vtable)._8_OnUpdate.methodPtr)
                    (this->instance(), deltaTime, this->instance()->klass->vtable._8_OnUpdate.method);
        }

        inline void Initialize() {
            external<void>(0x01d22cb0, this);
        }

        inline void PreparaNextV(bool isForward) {
            external<void>(0x01d235a0, this);
        }

        inline void PreparaNextH(bool isForward) {
            external<void>(0x01d23730, this);
        }

        inline void OnSubmit() {
            external<void>(0x01d23850, this);
        }

        inline void OnCancel() {
            external<void>(0x01d23980, this);
        }
    };
}
