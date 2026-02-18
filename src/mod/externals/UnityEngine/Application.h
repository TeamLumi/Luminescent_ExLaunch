#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct Application : ILClass<Application> {
        static inline void set_targetFrameRate(int32_t value) {
            external<void>(0x026A0240, value);
        }
    };
}
