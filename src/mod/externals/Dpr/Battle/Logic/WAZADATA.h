#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Pml/WazaData/WazaFlag.h"

namespace Dpr::Battle::Logic {
    struct WAZADATA : ILClass<WAZADATA> {
        static inline bool GetFlag(int32_t id, Pml::WazaData::WazaFlag flag) {
            return external<bool>(game::Address::Dpr_Battle_Logic_WAZADATA_GetFlag, id, flag);
        }

        static inline uint8_t GetType(int32_t id) {
            return external<uint8_t>(game::Address::Dpr_Battle_Logic_WAZADATA_GetType, id);
        }

        static inline int32_t GetSickPer(int32_t id) {
            return external<int32_t>(game::Address::Dpr_Battle_Logic_WAZADATA_GetSickPer, id);
        }

        static inline bool IsDamage(int32_t id) {
            return external<bool>(game::Address::Dpr_Battle_Logic_WAZADATA_IsDamage, id);
        }
    };
}