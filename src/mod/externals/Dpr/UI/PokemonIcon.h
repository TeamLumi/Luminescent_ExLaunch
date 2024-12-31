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

        static inline StaticILMethod<0x04c7f458> Method$$__Load__b__9_0 {};
        static inline StaticILMethod<0x04c7f450> Method$$__Load__b__10_1 {};
        static inline StaticILMethod<0x04c7f448, UnityEngine::Sprite> Method$$Load_10_0 {};

        static inline MethodInfo* Method$$BUISituationLoadIcon = nullptr;
        static MethodInfo* getMethod$$BUISituationLoadIcon(Il2CppMethodPointer method) {
            if (Method$$BUISituationLoadIcon == nullptr)
                Method$$BUISituationLoadIcon = (*Method$$Load_10_0)->copyWith(method);
            return Method$$BUISituationLoadIcon;
        };
    };
}
