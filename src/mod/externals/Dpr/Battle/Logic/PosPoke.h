#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BtlPokePos.h"

namespace Dpr::Battle::Logic {
    struct PosPoke : ILClass<PosPoke> {
        struct State : ILClass<State> {
            struct Fields {
                bool fEnable;
                uint8_t clientID;
                uint8_t existPokeID;
            };
        };

        struct Fields {
            PosPoke::State::Array* m_state;
            BtlPokePos_array* m_lastPosInst;
            BtlPokePos m_lastPosDmy;
        };

        inline bool IsExist(uint8_t pokeID) {
            return external<bool>(0x020d2540, this, pokeID);
        }
    };
}
