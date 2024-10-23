#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine::UI {
    struct Selectable;

    struct Navigation : ILStruct<Navigation> {
        struct Fields {
            int32_t m_Mode;
            Selectable* m_SelectOnUp;
            Selectable* m_SelectOnDown;
            Selectable* m_SelectOnLeft;
            Selectable* m_SelectOnRight;
        };
    };
}