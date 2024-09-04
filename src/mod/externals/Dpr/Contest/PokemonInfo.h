#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/String.h"

namespace Dpr::Contest {
    struct PokemonInfo : ILClass<PokemonInfo> {
        struct Fields {
            System::String::Object* nickname;
            bool isZIBAKOIRU;
            Pml::PokePara::PokemonParam::Object* pokeParam;
            int32_t size;
            uint8_t wazaType;
            int32_t wazaNo;
            int32_t haveItemNo;
            System::String::Object* wazaName;
            float wazaSeqTime;
            int32_t langID;
            Dpr::Battle::Logic::BTL_POKEPARAM::Object* btlPokeParam;
        };

        static_assert(offsetof(Fields, btlPokeParam) == 0x38);
    };
}
