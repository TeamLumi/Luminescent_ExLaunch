#pragma once

#include "externals/il2cpp-api.h"

#include "externals/BattleObjectEntity.h"
#include "externals/Dpr/Battle/View/SimpleParam.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/MaterialPropertyBlock.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector3.h"

struct BattlePokemonEntity : ILClass<BattlePokemonEntity> {
    struct RenderingParam : ILStruct<RenderingParam> {
        struct Fields {
            int32_t stencilRef;
            int32_t stencilComp;
            int32_t stencilOp;
        };
    };

    struct Fields : BattleObjectEntity::Fields {
        void* _animationPlayer; // AnimationPlayer_o*
        int32_t _size;
        float _scale;
        int32_t _landingType;
        int32_t _animationState;
        BattlePokemonEntity::RenderingParam::Object _renderingParam;
        void* _rendererClusters; // SmartPoint_Components_SkinnedMeshRendererCluster_array*
        void* _pokeAnimSound; // Dpr_PokeAnimSound_o*
        UnityEngine::MaterialPropertyBlock::Object* _propertyBlock;
        UnityEngine::Color::Object _fixMultiplierColor;
        Dpr::Battle::View::SimpleParam::Object _simpleParam;
        UnityEngine::Vector3::Object _adjustScale;
        UnityEngine::Transform::Object* _origin;
        void* _renderObjectMasks; // System_Collections_Generic_Dictionary_int__int__o*
        float _motionBlendTime;
        void* _PokemonMotionBlendTimeData_k__BackingField; // XLSXContent_BattleDataTable_SheetPokemonMotionBlendTime_o*
        bool isAnimationWaitAWhenDisable;
        int32_t waitALoopCount;
        bool _isPokeAnimSoundEnable;
        void* _PokemonVariation_k__BackingField; // PokemonVariation_o*
        void* _PokemonCustomNodeAnim_k__BackingField; // PokemonCustomNodeAnim_o*
        void* _PokemonAnime_k__BackingField; // PokemonAnime_o*
        void* _PokemonPrefabInfo_k__BackingField; // PokemonPrefabInfo_o*
        void* _patcheelPattern_k__BackingField; // Dpr_PatcheelPattern_o*
        bool isZIBAKOIRU;
    };

    inline void SetPatcheelPattern(uint32_t rand) {
        external<void>(0x01d77990, this, rand);
    }
};
