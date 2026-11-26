#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Events/UnityAction.h"

namespace Dpr::UI {
    struct UIInputButton : ILClass<UIInputButton> {
        struct Fields {
            UnityEngine::Events::UnityAction::Object* _onCallbacked;
            void* _input; // Dpr_UI_UIInputController_o*
            float _longPressTime;
            int32_t _button;
            float _pressTime;
        };

        inline void OnUpdate(float deltaTime) {
            external<void>(game::Address::Dpr_UI_UIInputButton_OnUpdate, this, deltaTime);
        }
    };
}
