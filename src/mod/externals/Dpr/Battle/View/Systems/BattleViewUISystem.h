#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::Battle::View::Systems {
    struct BattleViewUISystem : ILClass<BattleViewUISystem> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            // TODO
        };

        inline void PlaySe(int32_t eventId, MethodInfo* methodInfo) {
            external<void>(0x01e7c5f0, this, eventId, methodInfo);
        }
    };
}
