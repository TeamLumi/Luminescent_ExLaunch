#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct Random : ILClass<Random> {
        static inline int32_t Range(int32_t min, int32_t max) {
            return external<int32_t>(game::Address::UnityEngine_Random_Range_Int32_Int32, min, max);
        }

        static inline float Range(float min, float max) {
            return external<float>(game::Address::UnityEngine_Random_Range_Float_Float, min, max);
        }
    };
}
