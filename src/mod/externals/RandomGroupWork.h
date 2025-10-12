#pragma once

#include "externals/il2cpp-api.h"

struct RandomGroupWork : ILClass<RandomGroupWork> {
    static inline int32_t RandomValue(int32_t max) {
        return external<int32_t>(0x01998cb0, max);
    }

    static inline uint64_t GetRandom(int32_t no) {
        return external<uint64_t>(0x0199fac0, no);
    }

    static inline int32_t RandomRange(int32_t min, int32_t max) {
        return external<int32_t>(0x0199f410, min, max);
    }
};
