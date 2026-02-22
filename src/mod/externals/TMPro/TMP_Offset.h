#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Color32.h"

namespace TMPro {
    struct TMP_Offset : ILStruct<TMP_Offset> {
        struct Fields {
            float m_Left;
            float m_Right;
            float m_Top;
            float m_Bottom;
        };

        static_assert(sizeof(Fields) == 0x10);
    };
}
