#pragma once

#include "externals/il2cpp-api.h"

struct RandomGroupWork : ILClass<RandomGroupWork> {
    static inline int32_t RandomValue(int32_t max) {
        return external<int32_t>(0x01998cb0, max);
    }

    static inline float Value() {
        return external<float>(0x0199f420);
    }
};
