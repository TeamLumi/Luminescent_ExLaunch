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

    static_assert(offsetof(Fields, HandScale) == 0xc8);
    static_assert(sizeof(Fields) == 0x198);
};
