#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Color.h"

namespace UnityEngine::UI {
    struct ColorBlock : ILStruct<ColorBlock> {
        struct Fields {
            UnityEngine::Color::Object m_NormalColor;
            UnityEngine::Color::Object m_HighlightedColor;
            UnityEngine::Color::Object m_PressedColor;
            UnityEngine::Color::Object m_SelectedColor;
            UnityEngine::Color::Object m_DisabledColor;
            float m_ColorMultiplier;
            float m_FadeDuration;
        };
    };
}