#pragma once

#include "externals/il2cpp-api.h"

#include "externals/AnimationPlayer.h"
#include "externals/FieldObjectEntity.h"
#include "externals/OnlinePlayerCharacter.h"
#include "externals/UnityEngine/Vector2.h"

struct OpcManager;

struct OpcController : ILClass<OpcController> {
    struct Fields : OnlinePlayerCharacter::Fields {
        FieldObjectEntity::Object* _Entity;
        void* _PosList;                             // List<Vector2>
        void* _RotList;                             // List<float>
        void* _CharacterMove;                       // OpcCharacterMove
        AnimationPlayer::Object* _AnimationPlayer;
        float _RotY;
        bool _IsAnimStop;
        void* _CharaData;                           // OpcManager.CharaData (boxed struct)
        bool isUseDashAnimation;
        bool _isInitialized;
        float _moveThreshold;
        System::String::Object* _TalkLabel;
        UnityEngine::Vector2::Object _ContactSize;
        float _Speed;
        float _RotSpeed;
        float _TargetDistance;
        float _MinSpeed;
        float _MaxSpeed;
        float _AddSpeedRate;
        float _CurrentSpeed;
        float _CurrentDistance;
        float _IdleTransutuonTime;
    };

    // Access _Entity via Object*: ctrl->fields._Entity
    // (Can't use 'fields' in outer struct methods — only exists on Object)

    // SetCharaData(OpcManager::CharaData::Object data) @ 0x01e0fc90
    // Can't declare here — circular dependency with OpcManager.h.
    // Call directly: external<void>(0x01e0fc90, ctrl, charaData)

    inline void AddNextPosition(UnityEngine::Vector2::Object pos, float rotY) {
        external<void>(0x01e0fb80, this, pos, rotY);
    }

    inline void SetRotationY(float rotY) {
        external<void>(0x01e0fc10, this, rotY);
    }

    inline void ClearPos() {
        external<void>(0x01e0fc20, this);
    }
};

struct UgOpcController : ILClass<UgOpcController> {
    struct Fields : OpcController::Fields {
        void* _fieldObjectMove;  // 0xF0
        void* OnStartFinished;  // 0xF8 — Action callback
        bool isSwim;             // 0x100
    };
};
