#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/GridLayoutGroup.h"

namespace Dpr::UI {
    struct BoxListNavigate : ILClass<BoxListNavigate> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            int32_t _arrayWidth;
            int32_t _arrayHeight;
            UnityEngine::UI::GridLayoutGroup::Object* _layout;
            

        };
    };
}