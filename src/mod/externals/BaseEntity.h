#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector2Int.h"
#include "externals/UnityEngine/Vector3.h"

struct BaseEntity : ILClass<BaseEntity> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        System::String::Object* _enityName;
        bool _alreadyRegistered;
        float yawAngle;
        UnityEngine::Vector3::Object worldPosition;
        float savedYawAngle;
        UnityEngine::Vector3::Object savedPosition;
        UnityEngine::Vector3::Object _beforePosition_k__BackingField;
        UnityEngine::Transform::Object* _cacheTransform;
        int32_t _currentSequence_k__BackingField;
        int32_t _nextSequence_k__BackingField;
        float _sequenceTime_k__BackingField;
        BaseEntity::Object* _target_k__BackingField;
    };

    struct VirtualInvokeData_get_entityType {
        typedef System::String::Object*(*Il2CppMethodPointer)(BaseEntity::Object*, const MethodInfo*);
        Il2CppMethodPointer methodPtr;
        const MethodInfo* method;
    };

    struct VTable {
        VirtualInvokeData _0_Equals;
        VirtualInvokeData _1_Finalize;
        VirtualInvokeData _2_GetHashCode;
        VirtualInvokeData _3_ToString;
        VirtualInvokeData_get_entityType _4_get_entityType;
        VirtualInvokeData _5_GetAnimationPlayer;
        VirtualInvokeData _6_Awake;
        VirtualInvokeData _7_OnEnable;
        VirtualInvokeData _8_OnDisable;
        VirtualInvokeData _9_OnDestroy;
        VirtualInvokeData _10_OnUpdate;
        VirtualInvokeData _11_OnLateUpdate;
        VirtualInvokeData _12_SwitchToNext;
        VirtualInvokeData _13_ProcessSequence;
    };

    inline UnityEngine::Transform::Object* get_transform() {
        return external<UnityEngine::Transform::Object*>(0x01d66e50, this);
    }

    inline void SetPositionDirect(UnityEngine::Vector3::Object pos) {
        UnityEngine::Vector3::Fields posProxy = { .x = pos.fields.x, .y = pos.fields.y, .z = pos.fields.z };
        external<void>(0x01d67740, this, posProxy);
    }

    static_assert(sizeof(VTable) == 0xe0);
    static_assert(offsetof(VTable, _4_get_entityType) == 0x40);
    static_assert(offsetof(VTable, _5_GetAnimationPlayer) == 0x50);
};
