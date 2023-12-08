#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::Battle::View::UI {
    struct BUISituationButton : ILClass<BUISituationButton> {
        struct Fields : Dpr::Battle::View::UI::BUIButtonBase::Fields {
            UnityEngine::UI::Image::Object* _pokeIcon;
            UnityEngine::UI::Image::Object* _sexIcon;
            UnityEngine::UI::Image::Object* _statusIcon;
            TMPro::TextMeshProUGUI::Object* _trainerNameText;
        };

        inline bool Initialize(Dpr::Battle::Logic::BTL_POKEPARAM::Object* btlParam, System::String::Object* trainerName) {
            return external<bool>(0x01d22fb0, this, btlParam, trainerName);
        }
    };
}
