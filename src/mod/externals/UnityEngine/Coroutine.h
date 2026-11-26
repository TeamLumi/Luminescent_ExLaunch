#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/YieldInstruction.h"

namespace UnityEngine {
    struct Coroutine : ILClass<Coroutine, game::Address::UnityEngine_Coroutine_TypeInfo> {
        struct Fields : UnityEngine::YieldInstruction::Fields {
            intptr_t m_Ptr;
        };
    };
}
