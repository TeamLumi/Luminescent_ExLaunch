#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace Pml::PokePara {
    struct SerializedPokemonFull : ILStruct<SerializedPokemonFull, game::Address::Pml_PokePara_SerializedPokemonFull_Array_TypeInfo> {
        struct Fields {
            System::Byte_array* buffer;
        };

        static long GetByteCount() {
            return sizeof(System::Byte) * 0x158;
        }

        inline void CreateWorkIfNeed() {
            external<void>(game::Address::Pml_PokePara_SerializedPokemonFull_CreateWorkIfNeed, this);
        }
    };
}