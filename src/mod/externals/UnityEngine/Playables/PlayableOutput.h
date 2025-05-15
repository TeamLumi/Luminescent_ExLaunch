#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Playables/PlayableOutputHandle.h"

namespace UnityEngine::Playables {
    struct PlayableOutput : ILStruct<PlayableOutput> {
        struct Fields {
            PlayableOutputHandle::Object m_Handle;
        };
    };
}