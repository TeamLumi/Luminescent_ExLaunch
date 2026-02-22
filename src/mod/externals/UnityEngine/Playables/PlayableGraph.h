#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine::Playables {
    struct PlayableGraph : ILStruct<PlayableGraph> {
        struct Fields {
            intptr_t m_Handle;
            uint32_t m_Version;
        };
    };
}