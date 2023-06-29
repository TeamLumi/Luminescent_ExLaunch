#pragma once

#include "externals/il2cpp.h"

#include "externals/UnityEngine/AudioBehaviour.h"
#include "externals/UnityEngine/AudioClip.h"

namespace UnityEngine {
    struct AudioSource : ILClass<AudioSource> {
        struct Fields : UnityEngine::AudioBehaviour::Fields {

        };

        inline void Play(double delay) {
            external<void>(0x02c22500, this, delay);
        }

        inline void set_clip(UnityEngine::AudioClip::Object* value) {
            external<void>(0x02c227d0, this, value);
        }

        inline void set_loop(bool value) {
            external<void>(0x02c229c0, this, value);
        }

        inline void set_pitch(float value) {
            external<void>(0x02c22690, this, value);
        }

        inline void set_time(float value) {
            external<void>(0x02c22730, this, value);
        }

        inline void set_volume(float value) {
            external<void>(0x02c225f0, this, value);
        }
    };
}
