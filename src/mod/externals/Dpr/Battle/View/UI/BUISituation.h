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

        inline void Initialize() {
            external<void>(0x01d22cb0, this);
        }
    };
}
