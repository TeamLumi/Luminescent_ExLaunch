#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/Dpr/UI/PoketchButton.h"

namespace Dpr::UI {
    struct PoketchWindow;

    struct PoketchAppBase : ILClass<PoketchAppBase> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            Dpr::UI::PoketchButton::Array* Buttons;
            int32_t _AppID_k__BackingField;
            Dpr::UI::PoketchWindow* _poketchInstance;
            bool isInitialized;
            Dpr::UI::PoketchButton::Object* _PreButton_k__BackingField;
            int32_t _PreState_k__BackingField;
        };

        static_assert(offsetof(Fields, _PreState_k__BackingField) == 0x30);
        static_assert(sizeof(Fields) == 0x38);

        inline void OnUpdate(bool isAppControlEnable, Dpr::UI::PoketchButton::Object* currentButton, int32_t currentState) {
            external<void>(0x019ffa60, this, isAppControlEnable, currentButton, currentState);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$PoketchAppBase : List<List$$PoketchAppBase, Dpr::UI::PoketchAppBase> {

    };
}
