#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct ZukanLanguageButton : ILClass<ZukanLanguageButton> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::UI::Image::Object* image;
            int32_t index;
            UnityEngine::RectTransform::Object* rectTransform;
            UnityEngine::Sprite::Object* selectSprite;
            UnityEngine::Sprite::Object* unselectSprite;
            int32_t _LangId_k__BackingField;
        };
    };
}