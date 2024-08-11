#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct TypePanel : ILClass<TypePanel> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::UI::Image::Object* iconImage;
        };

        inline void Deactive() {
            external<void>(0x01857860, this);
        }
    };
}