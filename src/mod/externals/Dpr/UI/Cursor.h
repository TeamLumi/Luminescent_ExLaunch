#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct Cursor : ILClass<Cursor> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::UI::Image::Array* _frameImages;
            void* _frameSprites; // UnityEngine_Sprite_array*
            void* _animator; // UnityEngine_Animator_o*
            UnityEngine::Coroutine::Object* _opPlay;
        };

        inline void SetActive(bool enabled) {
            external<void>(0x01ec07b0, this, enabled);
        }
    };
}
