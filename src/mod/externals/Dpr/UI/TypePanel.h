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
            external<void>(game::Address::Dpr_UI_TypePanel_Deactive, this);
        }

        inline void Set(int32_t typeNo) {
            external<void>(game::Address::Dpr_UI_TypePanel_Set, this, typeNo);
        }
    };
}