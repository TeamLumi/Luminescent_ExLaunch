#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"
#include "externals/Pml/PokePara/CoreParam.h"

namespace Dpr::Battle::Logic {
    struct MyStatus : ILClass<MyStatus> {
        struct Fields {
            System::String::Object* name;
            bool sex;
            int32_t lang;
            uint32_t id;
            uint8_t fashion;
            uint8_t body_type;
            uint8_t hat;
            uint8_t shoes;
        };

        inline bool IsMyPokemon(Pml::PokePara::CoreParam* poke) {
            return external<bool>(game::Address::Dpr_Battle_Logic_MyStatus_IsMyPokemon, this, poke);
        }
    };
}
