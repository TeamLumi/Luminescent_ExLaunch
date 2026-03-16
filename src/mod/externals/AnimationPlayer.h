#pragma once

#include "externals/il2cpp-api.h"

struct AnimationPlayer : ILClass<AnimationPlayer> {
    struct Fields {
        // TODO
    };

    inline int32_t get_currentIndex() {
        return external<int32_t>(0x0211d8a0, this);
    }

    // Play(int index, float duration = 0, float startTime = 0)
    inline void Play(int32_t index, float duration = 0.0f, float startTime = 0.0f) {
        external<void>(0x0211e9b0, this, index, duration, startTime);
    }
};