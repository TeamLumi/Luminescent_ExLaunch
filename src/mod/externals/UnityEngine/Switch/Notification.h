#pragma once
#include "externals/il2cpp-api.h"

namespace UnityEngine::Switch {
    struct Notification : ILClass<Notification> {
        static inline void EnterExitRequestHandlingSection() {
            external<void>(0x02c2d1c0);
        }
    };
}