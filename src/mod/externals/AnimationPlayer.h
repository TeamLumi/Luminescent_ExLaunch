#pragma once

#include "externals/il2cpp-api.h"

struct AnimationPlayer : ILClass<AnimationPlayer> {
    struct Fields {
        void* _graph;           // 0x10 PlayableGraph (16 bytes)
        char _graph_pad[8];     // PlayableGraph is 16 bytes total
        void* _mixer;           // 0x20 Playable (16 bytes)
        char _mixer_pad[8];
        void* _output;          // 0x30 AnimationPlayableOutput (16 bytes)
        char _output_pad[8];
        void* _animator;        // 0x40 Animator*
        void* _playables;       // 0x48 AnimationClipPlayable[]
        float _increase;        // 0x50
        float _weight;          // 0x54
        int32_t _activePort;    // 0x58
        // ...rest omitted
    };

    inline int32_t get_currentIndex() {
        return external<int32_t>(0x0211d8a0, this);
    }

    // Play animation clip by index. duration=0 for instant switch.
    // Clip indices: 0=Idle, 1=Walk, 2=Run (see clip_names.h FIELD_MON_CLIPS)
    inline void Play(int32_t index, float duration = 0.0f, float startTime = 0.0f) {
        external<void>(0x0211e9b0, this, index, duration, startTime);
    }
};