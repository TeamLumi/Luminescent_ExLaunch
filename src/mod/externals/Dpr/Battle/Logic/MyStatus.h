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
            return external<bool>(0x0203d330, this, poke);
        }

        inline int32_t GetColorID() {
            return external<int32_t>(0x0203d3f0, this);
        }
    };
}
