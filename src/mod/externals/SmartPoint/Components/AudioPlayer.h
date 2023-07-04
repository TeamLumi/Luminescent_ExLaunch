#pragma once

#include "externals/il2cpp.h"

#include "externals/UnityEngine/AudioClip.h"

namespace SmartPoint::Components {
    struct AudioPlayer : ILClass<AudioPlayer> {
        struct Fields {

        };

        static inline SmartPoint::Components::AudioChannel::Object* PlayStreamDirect(int32_t channelIndex, UnityEngine::AudioClip::Object *clip, float time) {
            return external<SmartPoint::Components::AudioChannel::Object*>(0x02090b40, channelIndex, clip, time);
        }
    };
}
