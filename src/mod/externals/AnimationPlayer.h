#pragma once

#include "externals/il2cpp-api.h"

struct AnimationPlayer : ILClass<AnimationPlayer> {
    struct Fields {
        // TODO
    };

    inline int32_t get_currentIndex() {
        return external<int32_t>(0x0211d8a0, this);
    }

    // Play animation clip by index. duration=0 for instant switch.
    // Clip indices: 0=Idle, 1=Walk, 2=Run, 20=JumpStart, 21=JumpLoop, 22=JumpEnd
    inline void Play(int32_t index, float duration = 0.0f, float startTime = 0.0f) {
        external<void>(0x0211e9b0, this, index, duration, startTime);
    }
};