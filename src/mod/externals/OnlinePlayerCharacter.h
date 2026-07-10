#pragma once

#include "externals/il2cpp-api.h"

#include "externals/AnimationPlayer.h"
#include "externals/OpcState.h"
#include "externals/UnityEngine/MonoBehaviour.h"

struct OnlinePlayerCharacter : ILClass<OnlinePlayerCharacter> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        OpcState::Object* opcState;                 // 0x18
        void* balloon;                              // 0x20 Balloon
        void* emoticon;                             // 0x28 Emoticon
        void* _ShowWindow;                          // 0x30 Action<int>
        float talkDistance;                          // 0x38
        bool isMenuOpen;                            // 0x3C
        AnimationPlayer::Object* _myAnimationPlayer;// 0x40
        int32_t stationIndex;                       // 0x48
        float _IdleTransutuonTime;                  // 0x4C
        bool _IsTransitionIdle;                     // 0x50
        int32_t emoticonType;                       // 0x54
        bool isRecruiment;                          // 0x58
        bool isTransitionAfter;                     // 0x59
        bool isMyPlyaer;                            // 0x5A
    };
};
