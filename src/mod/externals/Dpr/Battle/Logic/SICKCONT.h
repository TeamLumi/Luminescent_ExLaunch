#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"

namespace Dpr::Battle::Logic {
    struct SICKCONT : ILClass<SICKCONT> {
        static inline Dpr::Battle::Logic::BTL_SICKCONT::Object MakeTurnParam(uint8_t causePokeID, uint8_t turns, uint16_t param) {
            return external<Dpr::Battle::Logic::BTL_SICKCONT::Object>(game::Address::Dpr_Battle_Logic_SICKCONT_MakeTurnParam, causePokeID, turns, param);
        }

        static inline Dpr::Battle::Logic::BTL_SICKCONT::Object MakeTurn(uint8_t causePokeID, uint8_t turns) {
            return external<Dpr::Battle::Logic::BTL_SICKCONT::Object>(game::Address::Dpr_Battle_Logic_SICKCONT_MakeTurn, causePokeID, turns);
        }
    };
}
