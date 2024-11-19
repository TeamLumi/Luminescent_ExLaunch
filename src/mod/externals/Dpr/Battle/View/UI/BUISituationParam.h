#pragma once

#include "externals/il2cpp-api.h"

#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Sprite.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::Battle::View::UI {
    struct BUISituationParam : ILClass<BUISituationParam> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            TMPro::TextMeshProUGUI::Object* _titleText;
            UnityEngine::UI::Image::Array* _arrowImages;
            UnityEngine::Sprite::Object* _upArrow;
            UnityEngine::Sprite::Object* _downArrow;
        };

        inline void Initialize(System::String::Object* paramName, int32_t value) {
            external<void>(0x01d24930, this, paramName, value);
        }
    };
}
