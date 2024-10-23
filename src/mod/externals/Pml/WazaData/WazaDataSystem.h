#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/WazaData/WazaDamageType.h"

namespace Pml::WazaData {
    struct WazaDataSystem : ILClass<WazaDataSystem, 0x04c64ef8> {
        inline static uint8_t GetType(int32_t id) {
            return external<uint8_t>(0x020533a0, id);
        };

        inline static WazaDamageType GetDamageType(int32_t id) {
            return external<WazaDamageType>(0x0205a110, id);
        };
    };
}