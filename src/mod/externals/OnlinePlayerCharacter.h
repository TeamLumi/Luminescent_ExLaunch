#pragma once

#include "externals/il2cpp-api.h"

#include "externals/AnimationPlayer.h"
#include "externals/OpcState.h"
#include "externals/UnityEngine/MonoBehaviour.h"

struct OnlinePlayerCharacter : ILClass<OnlinePlayerCharacter> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        OpcState::Object* opcState;
        void* balloon;                              // Balloon
        void* emoticon;                             // Emoticon
        void* _ShowWindow;                          // Action<int>
        float talkDistance;
        bool isMenuOpen;
        AnimationPlayer::Object* _myAnimationPlayer;
        int32_t stationIndex;
    };
};
