#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_RankReset : ILClass<Section_FromEvent_RankReset, 0x04c5b9d0> {
        struct Description : ILClass<Description, 0x04c5b360> {
            struct Fields {
                uint8_t pokeCount;
                System::Byte_array* pokeIDs;
            };

            inline void ctor() {
                external<void>(0x01fb7090, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
