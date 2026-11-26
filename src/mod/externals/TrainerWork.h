#pragma once

#include "externals/il2cpp-api.h"

#include "externals/MonsLv.h"

struct TrainerWork : ILClass<TrainerWork> {
    static inline bool GetWinFlag(int32_t index) {
        return external<bool>(game::Address::TrainerWork_GetWinFlag, index);
    }
};
