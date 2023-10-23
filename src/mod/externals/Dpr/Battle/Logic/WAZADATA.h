#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Pml/WazaData/WazaFlag.h"

namespace Dpr::Battle::Logic {
    struct WAZADATA : ILClass<WAZADATA> {
        static inline bool GetFlag(int32_t id, Pml::WazaData::WazaFlag flag) {
            return external<bool>(0x01f72bf0, id, flag);
        }
    };
}