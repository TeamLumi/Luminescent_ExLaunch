#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Audio/AudioInstance.h"
#include "externals/System/Action.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Events/UnityAction.h"

namespace Dpr::SubContents {
    struct Utils : ILClass<Utils, 0x04c59e88> {
        static inline Audio::AudioInstance::Object* PlayVoice(int32_t monsNo, int32_t formNo, int32_t voiceNo, UnityEngine::Events::UnityAction::Object* onFinished) {
            return external<Audio::AudioInstance::Object*>(0x01864180, monsNo, formNo, voiceNo, onFinished);
        }

        // Returns IEnumerator — pass result to Sequencer::Start()
        static inline Il2CppObject* LoadAsset(System::String::Object* path, System::Action::Object* onLoad, bool isVariant = false, bool useAssetUnloader = true, int32_t id = 0) {
            return external<Il2CppObject*>(0x01868950, path, onLoad, isVariant, useAssetUnloader, id);
        }
    };
}
