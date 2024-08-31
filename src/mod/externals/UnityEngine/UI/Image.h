#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/Sprite.h"
#include "externals/UnityEngine/UI/MaskableGraphic.h"

namespace UnityEngine::UI {
    struct Image : ILClass<Image> {
        struct Fields : UnityEngine::UI::MaskableGraphic::Fields {
            void* m_Sprite;
            void* m_OverrideSprite;
            int32_t m_Type;
            bool m_PreserveAspect;
            bool m_FillCenter;
            int32_t m_FillMethod;
            float m_FillAmount;
            bool m_FillClockwise;
            int32_t m_FillOrigin;
            float m_AlphaHitTestMinimumThreshold;
            bool m_Tracked;
            bool m_UseSpriteMesh;
            float m_PixelsPerUnitMultiplier;
            float m_CachedReferencePixelsPerUnit;
        };

        inline void set_sprite(UnityEngine::Sprite::Object* value) {
            external<void>(0x024b8ed0, this, value);
        }
    };
}
