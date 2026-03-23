#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::MsgWindow {
    struct MsgWindow : ILClass<MsgWindow> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            // Fields omitted — define as needed
        };
    };
}
