#pragma once

#include "externals/il2cpp-api.h"
#include "externals/BattleObjectEntity.h"
#include "externals/Dpr/Battle/View/SimpleParam.h"
#include "externals/Dpr/PatcheelPattern.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/Rendering/CompareFunction.h"
#include "externals/UnityEngine/Rendering/StencilOp.h"
#include "externals/UnityEngine/Vector3.h"

struct BattlePokemonEntity : ILClass<BattlePokemonEntity> {
    struct RenderingParam : ILStruct<RenderingParam> {
        struct Fields {
            int32_t stencilRef;
            UnityEngine::Rendering::CompareFunction stencilComp;
            UnityEngine::Rendering::StencilOp stencilOp;
        };
    };

    struct Fields : BattleObjectEntity::Fields {
        void* _animationPlayer; //AnimationPlayer_o*
        int32_t _size;
        float _scale;
        int32_t _landingType;
        int32_t _animationState;
        BattlePokemonEntity::RenderingParam::Object _renderingParam;
        void* _rendererClusters; //SmartPoint_Components_SkinnedMeshRendererCluster_array*
        void* _pokeAnimSound; //Dpr_PokeAnimSound_o*
        void* _propertyBlock; //UnityEngine_MaterialPropertyBlock_o*
        UnityEngine::Color::Object _fixMultiplierColor;
        Dpr::Battle::View::SimpleParam::Object _simpleParam;
        UnityEngine::Vector3::Object _adjustScale;
        void* _origin; //UnityEngine_Transform_o*
        void* _renderObjectMasks; //System_Collections_Generic_Dictionary_int__int__o*
        float _motionBlendTime;
        void* _PokemonMotionBlendTimeData_k__BackingField; //XLSXContent_BattleDataTable_SheetPokemonMotionBlendTime_o*
        bool isAnimationWaitAWhenDisable;
        int32_t waitALoopCount;
        bool _isPokeAnimSoundEnable;
        void* _PokemonVariation_k__BackingField; //PokemonVariation_o*
        void* _PokemonCustomNodeAnim_k__BackingField; //PokemonCustomNodeAnim_o*
        void* _PokemonAnime_k__BackingField; //PokemonAnime_o*
        void* _PokemonPrefabInfo_k__BackingField; //PokemonPrefabInfo_o*
        Dpr::PatcheelPattern::Object* _patcheelPattern_k__BackingField;
        bool isZIBAKOIRU;
    };

    static_assert(offsetof(Fields, isZIBAKOIRU) == 0x170);
};