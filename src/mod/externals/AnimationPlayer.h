#pragma once

#include "externals/il2cpp-api.h"

struct AnimationPlayer : ILClass<AnimationPlayer> {
    struct Fields {
        // TODO
    };

    inline int32_t get_currentIndex() {
        return external<int32_t>(game::Address::AnimationPlayer_get_currentIndex, this);
    }

    inline float get_currentRemaingTime() {
        return external<float>(game::Address::AnimationPlayer_get_currentRemaingTime, this);
    }

    inline bool get_IsPlayingEnd() {
        return external<bool>(game::Address::AnimationPlayer_get_IsPlayingEnd, this);
    }

    // This play an animation by its index, the supplied duration is the blending between the previous animation and new one in seconds.
    inline void Play(int32_t index, float duration = 0.0f, float startTime = 0.0f) {
        external<void>(game::Address::AnimationPlayer_Play, this, index, duration, startTime);
    }
};