#pragma once

#include "externals/il2cpp-api.h"

struct AnimationPlayer : ILClass<AnimationPlayer> {
    struct Fields {
        // TODO
    };

    inline int32_t get_currentIndex() {
        return external<int32_t>(0x0211d8a0, this);
    }

    inline float get_currentRemaingTime() {
        return external<float>(0x0211dae0, this);
    }

    inline bool get_IsPlayingEnd() {
        return external<bool>(0x0211de80, this);
    }

    // This play an animation by its index, the supplied duration is the blending between the previous animation and new one in seconds.
    // From what I found the base game always blends at .2
    inline void Play(int32_t index, float duration = 0.0f, float startTime = 0.0f) {
        external<void>(0x0211e9b0, this, index, duration, startTime);
    }
};