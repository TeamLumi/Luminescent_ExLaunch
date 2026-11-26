#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::UI {
    struct UIInputController : ILClass<UIInputController> {
        struct Fields {
            bool _inputEnabled;
            int32_t _autoPushButton;
        };

        inline bool IsPushButton(int32_t button, bool isForce) {
            return external<bool>(game::Address::Dpr_UI_UIInputController_IsPushButton, this, button, isForce);
        }
    };
}