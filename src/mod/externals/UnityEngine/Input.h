#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Touch.h"
#include "logger/logger.h"

namespace UnityEngine {
    struct Input : ILClass<Input> {
        static inline int32_t get_touchCount() {
            return external<int32_t>(game::Address::UnityEngine_Input_get_touchCount);
        }

        static inline void GetTouch(int32_t index, UnityEngine::Touch::Object* returnPtr) {
            external<void>(game::Address::UnityEngine_Input_GetTouch_Injected, index, returnPtr);
        }
    };
}
