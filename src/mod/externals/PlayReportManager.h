#pragma once

#include "externals/il2cpp-api.h"

struct PlayReportManager : ILStruct<PlayReportManager, 0x04c59f30> {
    struct Fields {
    };

    static inline void StartWildBattle(int32_t type) {
        external<void>(0x02ce9330, type);
    }
};
