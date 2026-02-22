#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct PokemonSick : ILClass<PokemonSick> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            void* _canvasGroup; // UnityEngine_CanvasGroup_o*
            UnityEngine::UI::Image::Object* _image;
            void* _fadeTw; // DG_Tweening_Core_TweenerCore_float__float__FloatOptions__o*
            int32_t _sick;
        };

        inline void Setup(Pml::PokePara::PokemonParam::Object* pokemonParam) {
            external<void>(0x01d93960, this, pokemonParam);
        }

        inline void Setup(Dpr::Battle::Logic::BTL_POKEPARAM::Object* btlParam) {
            external<void>(0x01d938e0, this, btlParam);
        }
    };
}
