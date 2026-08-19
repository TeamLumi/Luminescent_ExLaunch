#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct ExPokePos : ILClass<ExPokePos, 0x04c5a8e8> {
       static inline uint8_t ExpandPos(Object* exPokePos, int32_t btlType, uint8_t multiMode, BtlPokePos::Array* expandedPos)
        {
            return external<uint8_t>(0x018f67d0, exPokePos, btlType, multiMode, expandedPos);
        }

        static inline uint8_t ExpandExistPokeID(Object* exPokePos, int32_t btlType, uint8_t multiMode, POKECON::Object* pokeCon, System::Byte_array* pokeIDs)
        {
            return external<uint8_t>(0x018f7c30, exPokePos, btlType, multiMode, pokeCon, pokeIDs);
        }
    };
}