#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/PoketchAppBase.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/Material.h"

namespace Dpr::UI {
    struct PoketchAppEggChecker : ILClass<PoketchAppEggChecker> {
        struct Fields : Dpr::UI::PoketchAppBase::Fields {
            void* _pokeIcon; // Dpr_UI_PoketchEggCheckerPokeIcon_array*
            UnityEngine::UI::Image::Object* _eggImage;
            UnityEngine::UI::Image::Object* _blurImage;
            float _inBlurTime;
            float _outBlurTime;
            float _maxBlurSize;
            int32_t _state;
            float _blurTimeCount;
            UnityEngine::Material::Object* _matGrayScale;
            UnityEngine::Material::Object* _matBlur;
        };

        static_assert(offsetof(Fields, _matBlur) == 0x70);
        static_assert(sizeof(Fields) == 0x78);
    };
}