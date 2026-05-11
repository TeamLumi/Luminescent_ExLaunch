#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/UI/Image.h"

namespace UnityEngine {
    struct Mathf : ILClass<Mathf, 0x04c57278> { // Actually a struct, but ILStruct doesn't currently support TypeInfo
        static inline int32_t Min(int32_t a, int32_t b) {
            return external<int32_t>(0x026b3710, a, b);
        }

        static inline int32_t Max(int32_t a, int32_t b) {
            return external<int32_t>(0x026b8840, a, b);
        }

        static inline int32_t Clamp(int32_t value, int32_t min, int32_t max) {
            return external<int32_t>(0x026b8d90, value, min, max);
        }

        static inline float Log10(float f) {
            return external<float>(0x026b8950, f);
        }
    };
}