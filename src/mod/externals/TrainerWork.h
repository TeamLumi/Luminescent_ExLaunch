#pragma once

#include "externals/il2cpp-api.h"

#include "externals/MonsLv.h"

struct TrainerWork : ILClass<TrainerWork> {
    static inline bool GetWinFlag(int32_t index) {
        return external<bool>(0x018c18a0, index);
    }

    static inline void SetWinFlag(int32_t index) {
        external<void>(0x18C1670, index);
    }
};
