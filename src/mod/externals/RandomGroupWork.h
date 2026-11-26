#pragma once

#include "externals/il2cpp-api.h"

struct RandomGroupWork : ILClass<RandomGroupWork> {
    static inline int32_t RandomValue(int32_t max) {
        return external<int32_t>(game::Address::RandomGroupWork_RandomValue, max);
    }

    static inline float Value() {
        return external<float>(game::Address::RandomGroupWork_Value);
    }
};
