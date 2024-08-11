#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_PARTY.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"

namespace Dpr::Battle::Logic {
    struct MainModule;

    struct POKECON : ILClass<POKECON> {
        struct Fields {
            MainModule* m_mainModule;
            BTL_PARTY::Array* m_party;
            BTL_POKEPARAM::Array* m_activePokeParam;
            BTL_POKEPARAM::Array* m_storedPokeParam;
        };

        inline BTL_PARTY::Object* GetPartyData(uint32_t clientID) {
            return external<BTL_PARTY::Object*>(0x02038600, this, clientID);
        }

        inline Dpr::Battle::Logic::BTL_POKEPARAM::Object* GetPokeParamConst(uint8_t pokeID) {
            return external<Dpr::Battle::Logic::BTL_POKEPARAM::Object*>(0x02037250, this, pokeID);
        }
    };
}
