#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/Collections/IEnumerator.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/UnityEngine/Coroutine.h"

namespace SmartPoint::AssetAssistant {
    struct Sequencer : ILClass<Sequencer, 0x04c58888> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            // TODO
        };

        static inline UnityEngine::Coroutine::Object * Start(System::Collections::IEnumerator::Object *routine) {
            return external<UnityEngine::Coroutine::Object *>(0x02bc1db0, routine);
        }

        static inline void Stop(UnityEngine::Coroutine::Object *coroutine) {
            external<void>(0x02bc29b0, coroutine);
        }
    };
}
