#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_TurnCheck_Weather : ILClass<Section_TurnCheck_Weather, game::Address::Dpr_Battle_Logic_Section_TurnCheck_Weather_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_TurnCheck_Weather_Description_TypeInfo> {
            struct Fields {};

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_TurnCheck_Weather_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_TurnCheck_Weather_Result_TypeInfo> {
            struct Fields {
                bool isExpGet;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_TurnCheck_Weather_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
