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
};