#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Audio/AudioInstance.h"
#include "externals/UnityEngine/Events/UnityAction.h"

namespace Dpr::SubContents {
    struct Utils : ILClass<Utils, game::Address::Dpr_SubContents_Utils_TypeInfo> {
        static inline Audio::AudioInstance::Object* PlayVoice(int32_t monsNo, int32_t formNo, int32_t voiceNo, UnityEngine::Events::UnityAction::Object* onFinished) {
            return external<Audio::AudioInstance::Object*>(game::Address::Dpr_SubContents_Utils_PlayVoice, monsNo, formNo, voiceNo, onFinished);
        }
    };
}
