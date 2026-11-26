#pragma once

#include "externals/il2cpp.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "ShopBoutique.h"

namespace Dpr::UI {
    struct ShopBoutiqueChange : ILClass<ShopBoutiqueChange> {
        struct Fields : Dpr::UI::ShopBoutique::Fields {
            // TODO
        };

        static inline StaticILMethod<game::Address::Method_Dpr_UI_UIManager_CreateUIWindow_ShopBoutiqueChange, Dpr::UI::ShopBoutiqueChange> Method$$CreateUIWindow {};
    };
}
