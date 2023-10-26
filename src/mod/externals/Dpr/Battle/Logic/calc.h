#pragma once

#include "externals/il2cpp-api.h"

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
    };
}
