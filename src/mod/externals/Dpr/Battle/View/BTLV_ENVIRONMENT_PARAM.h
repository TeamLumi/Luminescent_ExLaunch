#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/Vector4.h"

namespace Dpr::Battle::View {
    struct BTLV_ENVIRONMENT_PARAM : ILStruct<BTLV_ENVIRONMENT_PARAM> {
        struct Fields {
            UnityEngine::Vector2::Object PitchYaw;
            UnityEngine::Vector4::Object CharacterLightColor;
            float CharacterLightIntensity;
            UnityEngine::Vector4::Object CharacterReflectorColor;
            UnityEngine::Vector4::Object PokeLightColor;
            float PokeLightIntensity;
            UnityEngine::Vector4::Object PokeReflectorColor;
        };

        static_assert(sizeof(Fields) == 0x50);
    };
}
