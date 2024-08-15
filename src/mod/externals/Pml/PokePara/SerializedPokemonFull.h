#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace Pml::PokePara {
    struct SerializedPokemonFull : ILStruct<SerializedPokemonFull, 0x04c59c08> {
        struct Fields {
            System::Byte_array* buffer;
        };

        static long GetByteCount() {
            return sizeof(System::Byte) * 0x158;
        }

        inline void CreateWorkIfNeed() {
            external<void>(0x020558c0, this);
        }
    };
}