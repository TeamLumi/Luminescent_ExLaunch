#pragma once

#include "externals/il2cpp-api.h"

#include "externals/AnimationPlayer.h"
#include "externals/Effect/EffectInstance.h"
#include "externals/FieldObjectEntity.h"
#include "externals/System/Func.h"
#include "externals/UnityEngine/MaterialPropertyBlock.h"
#include "externals/UnityEngine/Renderer.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector3.h"

struct FieldCharacterEntity : ILClass<FieldCharacterEntity, 0x04c5ece8> {
    struct Fields : FieldObjectEntity::Fields {
        float HandScale;
        AnimationPlayer::Object* _animationPlayer;
        void* _blinkPatterns; // CurvePatterns_o*
        void* _variations; // FieldCharacterVariation_array*
        int32_t _eyePatternIndex;
        int32_t _mouthPatternIndex;
        int32_t _currentVariation;
        UnityEngine::Renderer::Object* _watchRenderer;
        int32_t _blinkCurveIndex;
        float _blinkTime;
        int32_t _UVOffsetID;
        UnityEngine::MaterialPropertyBlock::Object* _propertyBlock;
        void* _effectCoolTime; // System::Single_array*
        UnityEngine::Vector3::Object NeckAngle;
        UnityEngine::Vector3::Object _updateNeckAngle;
        UnityEngine::Vector3::Object _updateNeckAngle2;
        UnityEngine::Transform::Object* _subductionNode;
        UnityEngine::Transform::Object* _hipNode;
        float SubductionDepth;
        Effect::EffectInstance::Object* SwimEffect;
        bool isPlayingSwimEffect;
        Effect::EffectInstance::Object* _swimWalkEffect;
        bool _isPlayingSwimWalkEffect;
        bool _IsForceBlink_k__BackingField;
        bool _reqestStopFootEffect;
        bool _isStopFootEffect;
        float _stopFootEffectRetrunTime;
        int32_t _oldAnimEventIndex;
        int32_t _oldAnimClipIndex;
        void* _animEvents; // XLSXContent_CharcterAnimeEvent_SheetanimeEvent_array_array*
        System::Func::Object* animeEndCallBack;
    };

    struct VirtualInvokeData_virtual_GetAnimationPlayer {
        typedef AnimationPlayer::Object*(*Il2CppMethodPointer)(FieldCharacterEntity::Object*, const MethodInfo*);
        Il2CppMethodPointer methodPtr;
        const MethodInfo* method;
    };

    struct VTable {
        VirtualInvokeData _0_Equals;
        VirtualInvokeData _1_Finalize;
        VirtualInvokeData _2_GetHashCode;
        VirtualInvokeData _3_ToString;
        VirtualInvokeData _4_get_entityType;
        VirtualInvokeData_virtual_GetAnimationPlayer _5_GetAnimationPlayer;
        VirtualInvokeData _6_Awake;
        VirtualInvokeData _7_OnEnable;
        VirtualInvokeData _8_OnDisable;
        VirtualInvokeData _9_OnDestroy;
        VirtualInvokeData _10_OnUpdate;
        VirtualInvokeData _11_OnLateUpdate;
        VirtualInvokeData _12_SwitchToNext;
        VirtualInvokeData _13_ProcessSequence;
        VirtualInvokeData _14_OnFootSE;
        VirtualInvokeData _15_OnFootEffect;
        VirtualInvokeData _16_GetIdleAnimationIndex;
    };

    inline AnimationPlayer::Object* virtual_GetAnimationPlayer() {
        return (*(this->instance()->klass->vtable)._5_GetAnimationPlayer.methodPtr)
               (this->instance(), this->instance()->klass->vtable._5_GetAnimationPlayer.method);
    }

    static_assert(sizeof(VTable) == 0x110);
    static_assert(offsetof(VTable, _4_get_entityType) == 0x40);
    static_assert(offsetof(VTable, _5_GetAnimationPlayer) == 0x50);

    static_assert(offsetof(Fields, HandScale) == 0xc8);
    static_assert(sizeof(Fields) == 0x198);
};
