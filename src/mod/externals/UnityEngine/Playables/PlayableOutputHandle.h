#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Playables/PlayableOutputHandle.h"

namespace UnityEngine::Playables {
    struct PlayableOutputHandle : ILStruct<PlayableOutputHandle> {
        struct Fields {
            intptr_t m_Handle;
            uint32_t m_Version;
        };
    };
}