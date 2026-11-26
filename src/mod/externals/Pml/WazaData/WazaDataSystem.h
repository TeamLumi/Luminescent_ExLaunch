#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/WazaData/WazaDamageType.h"

namespace Pml::WazaData {
    struct WazaDataSystem : ILClass<WazaDataSystem, game::Address::Pml_WazaData_WazaDataSystem_TypeInfo> {
        inline static uint8_t GetType(int32_t id) {
            return external<uint8_t>(game::Address::Pml_WazaData_WazaDataSystem_GetType, id);
        };

        inline static WazaDamageType GetDamageType(int32_t id) {
            return external<WazaDamageType>(game::Address::Pml_WazaData_WazaDataSystem_GetDamageType, id);
        };
    };
}