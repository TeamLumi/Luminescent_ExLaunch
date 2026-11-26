#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct PokemonIcon : ILClass<PokemonIcon> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::UI::Image::Object* _imageMonsIcon;
            UnityEngine::UI::Image::Object* _imageItemIcon;
            UnityEngine::UI::Image::Object* _imageBallDecoIcon;
        };

        static inline StaticILMethod<game::Address::Method_Dpr_UI_PokemonIcon_Load_b_10_0, UnityEngine::Sprite> Method$$Load_10_0 {};

        static inline MethodInfo* Method$$BUISituationLoadIcon = nullptr;
        static MethodInfo* getMethod$$BUISituationLoadIcon(Il2CppMethodPointer method) {
            if (Method$$BUISituationLoadIcon == nullptr)
                Method$$BUISituationLoadIcon = (*Method$$Load_10_0)->copyWith(method);
            return Method$$BUISituationLoadIcon;
        };
    };
}
