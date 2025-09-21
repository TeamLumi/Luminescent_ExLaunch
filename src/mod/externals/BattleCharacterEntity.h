#pragma once

#include "externals/il2cpp-api.h"

#include "externals/BattleObjectEntity.h"
#include "externals/ColorVariation.h"
#include "externals/Dpr/Battle/View/TrainerSimpleParam.h"

struct BattleCharacterEntity : ILClass<BattleCharacterEntity, 0x04c59f68> {
    struct Fields : BattleObjectEntity::Fields {
        void* _animationPlayer; // Dpr_Playables_BattleAnimationPlayer_o*
        ColorVariation::Object* _colorVariation;
        float _motionBlendTime;
        bool _isEnableAnimationPlayer;
        float _speakStartTime;
        Dpr::Battle::View::TrainerSimpleParam::Object _TrainerSimpleParam_k__BackingField;
    };

    struct VirtualInvokeData_Initialize {
        typedef void(*Il2CppMethodPointer)(BattleCharacterEntity::Object*, Dpr::Battle::View::TrainerSimpleParam::Object*, bool, const MethodInfo*);
        Il2CppMethodPointer methodPtr;
        const MethodInfo* method;
    };

    struct VTable {
        VirtualInvokeData _0_Equals;
        VirtualInvokeData _1_Finalize;
        VirtualInvokeData _2_GetHashCode;
        VirtualInvokeData _3_ToString;
        VirtualInvokeData _4_get_entityType;
        VirtualInvokeData _5_GetAnimationPlayer;
        VirtualInvokeData _6_Awake;
        VirtualInvokeData _7_OnEnable;
        VirtualInvokeData _8_OnDisable;
        VirtualInvokeData _9_OnDestroy;
        VirtualInvokeData _10_OnUpdate;
        VirtualInvokeData _11_OnLateUpdate;
        VirtualInvokeData _12_SwitchToNext;
        VirtualInvokeData _13_ProcessSequence;
        VirtualInvokeData _14_OnEarlyLateUpdate;
        VirtualInvokeData _15_OnPostLateUpdate;
        VirtualInvokeData_Initialize _16_Initialize;
        VirtualInvokeData _17_SetCaputureDemo;
        VirtualInvokeData _18_RequestSequence;
        VirtualInvokeData _19_SetRenderActive;
    };

    inline void Initialize(Dpr::Battle::View::TrainerSimpleParam::Object* param, bool isContest) {
        external<void>(0x01d68180, this, param, isContest);
    }

    inline void virtual_Initialize(Dpr::Battle::View::TrainerSimpleParam::Object* param, bool isContest) {
        (this->instance()->klass)->vtable._16_Initialize.methodPtr(((BattleCharacterEntity::Object*)this), param, isContest, (this->instance()->klass)->vtable._16_Initialize.method);
    }
};
