#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct Time : ILClass<Time> {
        static inline float get_deltaTime() {
            return external<float>(game::Address::UnityEngine_Time_get_deltaTime);
        }
    };
}