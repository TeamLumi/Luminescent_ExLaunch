#pragma once

#include "externals/il2cpp-api.h"

struct BtlTowerWork : ILClass<BtlTowerWork, 0x04c5a010> {

    static inline uint32_t GetRenshou(int32_t mode) {
        return external<uint32_t>(0x018e0d90, mode);
    }

    static inline int32_t GetRank(int32_t mode) {
        return external<int32_t>(0x018df1f0, mode);
    }

};
