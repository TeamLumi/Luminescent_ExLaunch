#pragma once

#include "externals/il2cpp-api.h"

#include "externals/TMPro/TMP_Offset.h"
#include "externals/UnityEngine/Color32.h"

namespace TMPro {
    struct HighlightState : ILStruct<HighlightState> {
        struct Fields {
            UnityEngine::Color32::Object color;
            TMP_Offset::Object padding;
        };

        static_assert(sizeof(Fields) == 0x14);
    };
}
