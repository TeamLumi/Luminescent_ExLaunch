#pragma once
#include "externals/il2cpp-api.h"

namespace UnityEngine::Switch {
    struct Notification : ILClass<Notification> {
        static inline void EnterExitRequestHandlingSection() {
            external<void>(game::Address::UnityEngine_Switch_Notification_EnterExitRequestHandlingSection);
        }
    };
}