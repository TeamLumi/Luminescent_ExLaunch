#pragma once

#include "externals/il2cpp-api.h"

#include "externals/FieldCharacterEntity.h"
#include "externals/Dpr/Battle/View/TrainerSimpleParam.h"

struct BattlePlayerEntity : ILClass<BattlePlayerEntity, 0x04c63a90> {
    struct Fields : BattleCharacterEntity::Fields {
        UnityEngine::Renderer::Object* _watchRenderer;
        UnityEngine::Renderer::Array* _hatRenderers;
        UnityEngine::Renderer::Array* _shoesRenderers;
        int32_t HatVariationParam;
        int32_t ShoesVariationParam;
        bool isCaputureDemo;
    };

    static_assert(offsetof(Fields, isCaputureDemo) == 0x110);
    static_assert(sizeof(Fields) == 0x118);
};