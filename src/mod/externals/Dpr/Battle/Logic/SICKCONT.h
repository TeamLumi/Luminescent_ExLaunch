#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"

namespace Dpr::Battle::Logic {
    struct SICKCONT : ILClass<SICKCONT> {
        static inline Dpr::Battle::Logic::BTL_SICKCONT::Object MakeTurnParam(uint8_t causePokeID, uint8_t turns, uint16_t param) {
            return external<Dpr::Battle::Logic::BTL_SICKCONT::Object>(0x020d46f0, causePokeID, turns, param);
        }

        static inline Dpr::Battle::Logic::BTL_SICKCONT::Object MakeTurn(uint8_t causePokeID, uint8_t turns) {
            return external<Dpr::Battle::Logic::BTL_SICKCONT::Object>(0x020d4680, causePokeID, turns);
        }

        static inline Dpr::Battle::Logic::BTL_SICKCONT::Object MakePokeTurn(uint8_t causePokeID, uint8_t pokeID, uint8_t turns) {
            return external<Dpr::Battle::Logic::BTL_SICKCONT::Object>(0x020d4960, causePokeID, pokeID, turns);
        }

        static inline uint8_t GetPokeID(Dpr::Battle::Logic::BTL_SICKCONT::Object* cont) {
            return external<uint8_t>(0x020d4b40, cont);
        }
    };
}
