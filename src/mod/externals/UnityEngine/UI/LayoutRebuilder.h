#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/RectTransform.h"

namespace UnityEngine::UI {
    struct LayoutRebuilder : ILClass<LayoutRebuilder, game::Address::UnityEngine_UI_LayoutRebuilder_TypeInfo> {
        struct Fields {
            UnityEngine::RectTransform::Object* m_ToRebuild;
            int32_t m_CachedHashFromTransform;
        };

        static_assert(offsetof(Fields, m_CachedHashFromTransform) == 0x8);

        static inline void ForceRebuildLayoutImmediate(UnityEngine::RectTransform::Object* layoutRoot) {
            external<void>(game::Address::UnityEngine_UI_LayoutRebuilder_ForceRebuildLayoutImmediate, layoutRoot);
        }
    };
}
