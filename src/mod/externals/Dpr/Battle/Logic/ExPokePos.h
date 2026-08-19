#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BtlPokePos.h"
#include "externals/Dpr/Battle/Logic/POKECON.h"

namespace Dpr::Battle::Logic {
    struct ExPokePos : ILClass<ExPokePos, 0x04c5a8e8> {

        inline void ctor(int32_t type, uint8_t basePos) {
            external<void>(0x018f6730, this, type, basePos);
        }

        static inline uint8_t ExpandExistPokeID(Object* exPokePos, int32_t btlType, uint8_t multiMode, POKECON::Object* pokeCon, System::Byte_array* pokeIDs)
        {
            return external<uint8_t>(0x018f7c30, exPokePos, btlType, multiMode, pokeCon, pokeIDs);
        }
    };
}