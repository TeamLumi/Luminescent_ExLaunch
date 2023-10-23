#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"

namespace Dpr::Battle::Logic {
    struct SICKCONT : ILClass<SICKCONT> {
        static inline Dpr::Battle::Logic::BTL_SICKCONT::Object MakeTurnParam(uint8_t causePokeID, uint8_t turns, uint16_t param) {
            return external<Dpr::Battle::Logic::BTL_SICKCONT::Object>(0x020d46f0, causePokeID, turns, param);
        }
    };
}
