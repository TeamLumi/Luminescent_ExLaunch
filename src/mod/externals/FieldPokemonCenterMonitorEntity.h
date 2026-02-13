#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/PokemonIcon.h"
#include "externals/FieldEventEntity.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/UI/Image.h"

struct FieldPokemonCenterMonitorEntity : ILClass<FieldPokemonCenterMonitorEntity> {
    struct Fields : FieldEventEntity::Fields {
        void* canvas;
        Dpr::UI::PokemonIcon::Array* pokemonIcons;
        float defaultScale;
        float animationScale;
        UnityEngine::UI::Image::Array* pokemonImages;
        int32_t currentDisplayIndex;
        float animationTime;
        float progressTime;
        bool isPlayingHealAnimation;
    };

    static_assert(offsetof(Fields, isPlayingHealAnimation) == 0x15C);
};