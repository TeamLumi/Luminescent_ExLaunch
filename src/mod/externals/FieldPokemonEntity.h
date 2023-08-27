#pragma once

#include "externals/il2cpp-api.h"

#include "externals/FieldObjectEntity.h"
#include "externals/System/Primitives.h"
#include "externals/UnityEngine/MaterialPropertyBlock.h"
#include "externals/UnityEngine/Transform.h"

struct FieldPokemonEntity : ILClass<FieldPokemonEntity> {
    struct Fields : FieldObjectEntity::Fields {
        float scale;
        void* _animationPlayer; // AnimationPlayer_o*
        void* _rendererClusters; // SmartPoint_Components_SkinnedMeshRendererCluster_array*
        void* _pokeAnimSound; // Dpr_PokeAnimSound_o*
        System::Boolean_array* _blinkEnables;
        bool hasLandingSequence;
        int32_t eyeIndex;
        bool autoBlinkEnable;
        bool updateEnable;
        UnityEngine::MaterialPropertyBlock::Object* _propertyBlock;
        UnityEngine::Transform::Object* _originBone;
        void* _PrefabInfo_k__BackingField; // PokemonPrefabInfo_o*
        System::Int32_array* _blinkPatterns;
        float _blinkIntervalTime;
        float _blinkTime;
        int32_t _blinkIndex;
        bool _enablePlayInitialIdleAnimation;
        void* _animator; // UnityEngine_Animator_o*
        bool reOnEnable;
        int32_t wait;
    };
};
