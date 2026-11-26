#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct WazaCategoryPanel : ILClass<WazaCategoryPanel> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::UI::Image::Object* iconImage;
        };

        static_assert(offsetof(Fields, iconImage) == 0x8);

        inline void Deactive() {
            external<void>(game::Address::Dpr_UI_WazaCategoryPanel_Deactive, this);
        }
    };
}