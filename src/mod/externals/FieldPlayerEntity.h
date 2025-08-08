#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Audio/AudioInstance.h"
#include "externals/Effect/EffectInstance.h"
#include "externals/FieldCharacterEntity.h"
#include "externals/JumpCalculator.h"
#include "externals/System/Action.h"
#include "externals/System/Func.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Renderer.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/Vector3.h"
#include "externals/XLSXContent/MapAttributeTable.h"

struct FieldPlayerEntity : ILClass<FieldPlayerEntity> {
    struct Fields : FieldCharacterEntity::Fields {
        UnityEngine::Renderer::Array* _hatRenderers;
        UnityEngine::Renderer::Array* _shoesRenderers;
        UnityEngine::GameObject::Object* _meshGroup;
        UnityEngine::GameObject::Object* _bicycleObject;
        UnityEngine::Renderer::Array* _rodRenderers;
        UnityEngine::Renderer::Object* _podRenderer;
        UnityEngine::Renderer::Object* _beadaruRenderer;
        UnityEngine::Renderer::Object* _mukuhawkRenderer;
        UnityEngine::Color::Array* _bicycleColors;
        UnityEngine::Renderer::Object* _bicycleRenderer;
        int32_t _bicycleMaterialIndex;
        int32_t _bicycleColorIndex_k__BackingField;
        bool isExtrudable;
        bool _DashFlag_k__BackingField;
        JumpCalculator::Object* _path;
        bool _setupMaterials;
        bool _hit_se_request;
        float _hit_se_wait;
        UnityEngine::Vector3::Object InputMoveVector;
        System::Func::Object* _LateUpdateEvent_k__BackingField;
        float _MoveSpeed_k__BackingField;
        float _beforeAngle;
        int32_t FormType;
        XLSXContent::MapAttributeTable::SheetData::Object* nowAttribute;
        bool isAttributeStop;
        int32_t _FashionTableID_k__BackingField;
        bool FootSeWalkStartEvent;
        void* CheckGridCollision; // FieldPlayerEntity_CheckGridCollisionFunc_o*
        int32_t _sandFallSeq;
        float _sandFallWait;
        float _sandFallPosZ;
        bool UpdateInputAngleDisable;
        int32_t KairikiPushObjectIndex;
        float KairikiPushTime;
        float debugInputTime;
        float debugInputVectolX;
        bool _isCrossUpdate;
        UnityEngine::Vector2::Object _crossInputVectol;
        UnityEngine::Vector2::Object _crossStopPosition;
        int32_t _crossInputDir;
        float _crossKey_pushTime;
        UnityEngine::Vector3::Object _eventMoveTargetPosition;
        float _eventMoveTargetAngle;
        bool _eventMoveEnd;
        bool _eventRotateEnd;
        int32_t currentForm;
        int32_t nextForm;
        System::Action::Object* formFinish;
        bool changeFormRetInput;
        void* _bicJump; // BicycleJump_o*
        bool _isCycDownHillMode;
        float _bicOldAcceleration;
        float _bicAccelerationTime;
        float _bicDecelerateTime;
        bool _isBicMaxSpeed;
        int32_t IceFloorState;
        UnityEngine::Vector3::Object IceFloorDirection;
        int32_t IceSlidingLevel;
        float IceSlidingSpeed;
        bool IsIceSlope;
        float IceFloorStopTime;
        UnityEngine::Vector3::Object IceFloorStartGridCenterPos;
        void* IceSlopeDownMove; // Dpr_FieldObjectMove_o*
        void* IceSlidingRotate; // Dpr_FieldObjectRotateYaw_o*
        int32_t IceFloorDirtyNextDir;
        int32_t CurrentMoveFloor;
        int32_t NextMoveFloor;
        float MoveFloorTime;
        float PrevRotateOffset;
        void* MoveFloorRotate; // Dpr_FieldObjectRotateYaw_o*
        bool ForcePlayNaminoriEffect;
        Effect::EffectInstance::Object* NaminoriEffect;
        Audio::AudioInstance::Object* NaminoriAudio;
        bool IsPlayNaminoriEffect;
        float NaminoriSeWait;
        int32_t NaminoriEventRequest;
        void* CheckGridCollisionCheckSwim; // FieldPlayerEntity_CheckGridCollisionCheckSwimFunc_o*
        void* CheckGridCollisionCalcSwim; // FieldPlayerEntity_CheckGridCollisionCalcSwimFunc_o*
        void* CheckGridCollisionEndSwim; // FieldPlayerEntity_CheckGridCollisionEndSwimFunc_o*
        void* CheckGridCollisionCalcSwimEnd; // FieldPlayerEntity_CheckGridCollisionCalcSwimEndFunc_o*
        UnityEngine::Transform::Object* _BiidaruTransform_k__BackingField;
        UnityEngine::Vector3::Object BiidaruPosOriginal;
        UnityEngine::Vector3::Object BiidaruMoveStartPos;
        UnityEngine::Vector3::Object BiidaruMoveEndPos;
        void* BiidaruMoveTime; // Dpr_FieldFloatMove_o*
        void* _VisiblePodMove_k__BackingField; // Dpr_FieldFloatMove_o*
        Effect::EffectInstance::Object* RockClimbEffect;
        int32_t SwampDeepInputDir;
        int32_t SwampDeepInputCount;
        int32_t SwampPhase;
        void* SwampMove; // Dpr_FieldObjectMove_o*
        void* SwampWait; // Dpr_FieldFloatMove_o*
        bool isPlayedSwampStartEffect;
        bool isSwampLoopEffect;
    };

    static inline void set_colorID(int32_t value) {
        external<void>(0x02cef870, value);
    }

    inline void GetInputVector(UnityEngine::Vector2::Object* stickL, float* stickPowerSq, float deltatime, bool* analogstick) {
        external<void>(0x01dabb10, this, stickL, stickPowerSq, deltatime, analogstick);
    }

    inline void PlayJumpStart() {
        external<void>(0x01da7b50, this);
    }

    inline void PlayJumpLoop() {
        external<void>(0x01da9ed0, this);
    }

    inline void PlayJumpEnd() {
        external<void>(0x01da9fc0, this);
    }

    inline bool IsRideBicycle() {
        return external<bool>(0x01da7c70, this);
    }

    inline void StopCrossInputAndBicycle() {
        external<void>(0x01da4ed0, this);
    }

    static_assert(offsetof(Fields, _hatRenderers) == 0x198);
    static_assert(offsetof(Fields, _path) == 0x1F8);
    static_assert(sizeof(Fields) == 0x3e8);
};