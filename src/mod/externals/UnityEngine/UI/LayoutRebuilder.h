#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/RectTransform.h"

namespace UnityEngine::UI {
    struct LayoutRebuilder : ILClass<LayoutRebuilder, 0x04c58388> {
        struct Fields {
            UnityEngine::RectTransform::Object* m_ToRebuild;
            int32_t m_CachedHashFromTransform;
        };

        static_assert(offsetof(Fields, m_CachedHashFromTransform) == 0x8);

        static inline void ForceRebuildLayoutImmediate(UnityEngine::RectTransform::Object* layoutRoot) {
            external<void>(0x02557850, layoutRoot);
        }
    };
}
