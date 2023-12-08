#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DG/Tweening/Core/ABSSequentiable.h"
#include "externals/DG/Tweening/TweenCallback.h"
#include "externals/System/String.h"

namespace DG::Tweening {
    struct Tween : ILClass<Tween> {
        struct Fields : DG::Tweening::Core::ABSSequentiable::Fields {
            float timeScale;
            bool isBackwards;
            Il2CppObject* id;
            System::String::Object* stringId;
            int32_t intId;
            Il2CppObject* target;
            int32_t updateType;
            bool isIndependentUpdate;
            DG::Tweening::TweenCallback::Object* onPlay;
            DG::Tweening::TweenCallback::Object* onPause;
            DG::Tweening::TweenCallback::Object* onRewind;
            DG::Tweening::TweenCallback::Object* onUpdate;
            DG::Tweening::TweenCallback::Object* onStepComplete;
            DG::Tweening::TweenCallback::Object* onComplete;
            DG::Tweening::TweenCallback::Object* onKill;
            DG::Tweening::TweenCallback::Object* onWaypointChange;
            bool isFrom;
            bool isBlendable;
            bool isRecyclable;
            bool isSpeedBased;
            bool autoKill;
            float duration;
            int32_t loops;
            int32_t loopType;
            float delay;
            bool _isRelative_k__BackingField;
            int32_t easeType;
            void* customEase; // DG_Tweening_EaseFunction_o*
            float easeOvershootOrAmplitude;
            float easePeriod;
            System::String::Object* debugTargetId;
            void* typeofT1; // System_Type_o*
            void* typeofT2; // System_Type_o*
            void* typeofTPlugOptions; // System_Type_o*
            bool _active_k__BackingField;
            bool isSequenced;
            void* sequenceParent; // DG_Tweening_Sequence_o*
            int32_t activeId;
            int32_t specialStartupMode;
            bool creationLocked;
            bool startupDone;
            bool _playedOnce_k__BackingField;
            float _position_k__BackingField;
            float fullDuration;
            int32_t completedLoops;
            bool isPlaying;
            bool isComplete;
            float elapsedDelay;
            bool delayComplete;
            int32_t miscInt;
        };
    };
}
