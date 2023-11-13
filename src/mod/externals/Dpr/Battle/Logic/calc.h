#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Pml/WazaData/WazaSick.h"

namespace Dpr::Battle::Logic {
    struct calc : ILClass<calc, 0x04c5a768> {
        static inline uint32_t PERSONAL_GetParam(int32_t monsno, int32_t formno, int32_t paramID) {
            return external<uint32_t>(0x01f75510, monsno, formno, paramID);
        }

        static inline bool PERSONAL_IsEvoCancelPokemon(int32_t monsno, int32_t formno, uint8_t level) {
            return external<bool>(0x01f75630, monsno, formno, level);
        }

        static inline uint32_t RandRange(uint32_t min, uint32_t max) {
            return external<uint32_t>(0x01f74a10, min, max);
        }

        static inline uint32_t GetRand(uint32_t range) {
            return external<uint32_t>(0x01f74990, range);
        }

        static inline BTL_SICKCONT::Object MakeDefaultPokeSickCont(Pml::WazaData::WazaSick sick, uint8_t causePokeID, bool isCantUseRand) {
            return external<BTL_SICKCONT::Object>(0x01f74d20, sick, causePokeID, isCantUseRand);
        }
    };
}
