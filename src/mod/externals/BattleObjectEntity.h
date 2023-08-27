#pragma once

#include "externals/il2cpp-api.h"

#include "externals/BaseEntity.h"
#include "externals/RendererInfo.h"
#include "externals/UnityEngine/Renderer.h"
#include "externals/UnityEngine/Transform.h"

struct BattleObjectEntity : ILClass<BattleObjectEntity> {
    struct Fields : BaseEntity::Fields {
        void* _blinkProcess; // BattleObjectEntity_CharaAutomaticBlinkProcess_o*
        UnityEngine::Renderer::Array* _renderers;
        UnityEngine::Transform::Array* _locators;
        int32_t _modelEntityType;
        float _animationSpeed;
        void* _shadowCastSystem; // Dpr_Battle_View_Systems_BattleShadowCastSystem_o*
        void* _animator; // UnityEngine_Animator_o*
        RendererInfo::Array* _rendererInfos;
    };
};
