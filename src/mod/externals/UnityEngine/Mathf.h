#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/UI/Image.h"

namespace UnityEngine {
    struct Mathf : ILClass<Mathf, game::Address::UnityEngine_Mathf_TypeInfo> { // Actually a struct, but ILStruct doesn't currently support TypeInfo
        static inline int32_t Min(int32_t a, int32_t b) {
            return external<int32_t>(game::Address::UnityEngine_Mathf_Min, a, b);
        }

        static inline int32_t Max(int32_t a, int32_t b) {
            return external<int32_t>(game::Address::UnityEngine_Mathf_Max, a, b);
        }

        static inline int32_t Clamp(int32_t value, int32_t min, int32_t max) {
            return external<int32_t>(game::Address::UnityEngine_Mathf_Clamp, value, min, max);
        }

        static inline float Log10(float f) {
            return external<float>(game::Address::UnityEngine_Mathf_Log10, f);
        }
    };
}