#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct WazaEffectParams : ILClass<WazaEffectParams> {
        struct Fields {
            int32_t effectWazaID;
            uint8_t attackerPos;
            uint8_t targetPos;
            uint8_t effectIndex;
            uint8_t pluralHitIndex;
            bool isGShockOccur;
            bool fEnable;
            bool fDone;
            uint16_t commandQueuePos;
            bool isSyncDamageEffect;
            uint8_t subEff_pokeCnt;
            uint8_t subEff_pokeID_1;
            uint8_t subEff_pokeID_2;
            uint8_t subEff_pokeID_3;
            uint8_t subEff_pokeID_4;
            uint8_t subEff_pokeID_5;
        };
    };
}
