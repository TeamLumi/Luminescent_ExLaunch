#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Sprite.h"

namespace UnityEngine::UI {
    struct SpriteState : ILStruct<SpriteState> {
        struct Fields {
            UnityEngine::Sprite::Object* m_HighlightedSprite;
            UnityEngine::Sprite::Object* m_PressedSprite;
            UnityEngine::Sprite::Object* m_SelectedSprite;
            UnityEngine::Sprite::Object* m_DisabledSprite;
        };
    };
}