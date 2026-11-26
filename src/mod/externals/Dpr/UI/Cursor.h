#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/Coroutine.h"

namespace Dpr::UI {
    struct Cursor : ILClass<Cursor, game::Address::Dpr_UI_Cursor_TypeInfo> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::UI::Image::Array* _frameImages;
            void* _frameSprites;
            void* _animator;
            UnityEngine::Coroutine::Object* _opPlay;
        };

        inline void SetActive(bool enabled) {
            external<void>(game::Address::Dpr_UI_Cursor_SetActive, this, enabled);
        }

        inline void SetFrameSprite(int32_t index) {
            external<void>(game::Address::Dpr_UI_Cursor_SetFrameSprite, this, index);
        }
    };
}