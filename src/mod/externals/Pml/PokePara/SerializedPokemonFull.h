#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace Pml::PokePara {
    struct SerializedPokemonFull : ILStruct<SerializedPokemonFull> {
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

    static Il2CppClass* SerializedPokemonFull_array_TypeInfo() {
        return *reinterpret_cast<Il2CppClass**>(exl::util::modules::GetTargetOffset(0x04c59c08));
    }

}