#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct BattleCounter : ILClass<BattleCounter> {
        struct Fields {
            void* m_uniqueCount; // System_UInt64_array*
            void* m_clientCount; // System_UInt64_array_array*
            void* m_sideCount; // System_UInt64_array_array*
        };

        inline uint64_t Get(uint8_t counterID) {
            return external<uint64_t>(0x018794a0, this, counterID);
        }
    };
}
