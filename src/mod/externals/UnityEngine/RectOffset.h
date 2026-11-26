#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct RectOffset : ILClass<RectOffset> {
        struct Fields {
            intptr_t m_Ptr;
            Il2CppObject* m_SourceStyle;
        };

        inline int32_t get_top() {
            return external<int32_t>(game::Address::UnityEngine_RectOffset_get_top, this);
        }

        inline int32_t get_bottom() {
            return external<int32_t>(game::Address::UnityEngine_RectOffset_get_bottom, this);
        }
    };
}
