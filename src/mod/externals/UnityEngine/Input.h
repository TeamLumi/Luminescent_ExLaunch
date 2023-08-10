#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Touch.h"

namespace UnityEngine {
    struct Input : ILClass<Input> {
        static inline int32_t get_touchCount() {
            return external<int32_t>(0x02c2b130);
        }

        static inline UnityEngine::Touch::Object GetTouch(int32_t index) {
            return external<UnityEngine::Touch::Object>(0x02c2abb0, index);
        }
    };
}
