#pragma once

#include "externals/il2cpp.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "ShopBoutique.h"

namespace Dpr::UI {
    struct ShopBoutiqueChange : ILClass<ShopBoutiqueChange> {
        struct Fields : Dpr::UI::ShopBoutique::Fields {
            // TODO
        };

        static inline StaticILMethod<0x04c8ffe8, Dpr::UI::ShopBoutiqueChange> Method$$CreateUIWindow {};
    };
}
