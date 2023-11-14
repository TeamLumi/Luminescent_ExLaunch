#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Pml/WazaData/WazaFlag.h"

namespace Dpr::Battle::Logic {
    struct WAZADATA : ILClass<WAZADATA> {
        static inline bool GetFlag(int32_t id, Pml::WazaData::WazaFlag flag) {
            return external<bool>(0x01f72bf0, id, flag);
        }

        static inline uint8_t GetType(int32_t id) {
            return external<uint8_t>(0x01f72b20, id);
        }

        static inline int32_t GetSickPer(int32_t id) {
            return external<int32_t>(0x01f72b10, id);
        }
    };
}