#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/WazaParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FreeFall_Release : ILClass<Section_FreeFall_Release, game::Address::Dpr_Battle_Logic_Section_FreeFall_Release_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FreeFall_Release_Description_TypeInfo> {
            struct Fields {
                BTL_POKEPARAM::Object* attacker;
                bool canAppearSelf;
                bool canAppearTarget;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FreeFall_Release_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FreeFall_Release_Result_TypeInfo> {
            struct Fields {};

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FreeFall_Release_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
