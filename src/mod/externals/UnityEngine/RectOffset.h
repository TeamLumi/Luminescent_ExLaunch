#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct RectOffset : ILClass<RectOffset> {
        struct Fields {
            intptr_t m_Ptr;
            Il2CppObject* m_SourceStyle;
        };

        inline int32_t get_top() {
            return external<int32_t>(0x02695e30, this);
        }

        inline int32_t get_bottom() {
            return external<int32_t>(0x02695e80, this);
        }
    };
}
