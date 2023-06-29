#pragma once

#include "externals/il2cpp.h"

#include "externals/UnityEngine/AudioSource.h"

namespace SmartPoint::Components {
    struct AudioChannel : ILClass<AudioChannel> {
        struct Fields {
            int32_t _type;
            UnityEngine::AudioSource::Object* _source;
            float _volume;
            int32_t _status_k__BackingField;
            float _duration_k__BackingField;
            float _elapsedTime_k__BackingField;
        };
    };
}
