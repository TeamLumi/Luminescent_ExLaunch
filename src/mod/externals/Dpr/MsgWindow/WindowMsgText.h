#pragma once

#include "externals/il2cpp-api.h"

#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/RectTransform.h"

namespace Dpr::MsgWindow {
    struct WindowMsgText : ILClass<WindowMsgText> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            TMPro::TextMeshProUGUI::Object* text;
            UnityEngine::RectTransform::Object* textRect;
            UnityEngine::Color::Object defaultColor;
        };

        static_assert(offsetof(Fields, defaultColor) == 0x18);
    };

}