#pragma once

#include "externals/il2cpp.h"

#include "externals/UnityEngine/MonoBehaviour.h"

namespace Audio {
    struct AudioManager;
}

namespace Dpr {
    namespace Message{
        struct MessageManager;
    }

    namespace UI {
        struct UIManager;
    }
}

namespace SmartPoint::AssetAssistant {
    struct Sequencer;
}

namespace SmartPoint::AssetAssistant {
    struct SingletonMonoBehaviour : ILClass<SingletonMonoBehaviour, 0x04c59b28> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {};

        static inline StaticILMethod<0x04c676a0, Audio::AudioManager> Method$$AudioManager$$get_Instance {};
        static inline StaticILMethod<0x04c67700, Dpr::UI::UIManager> Method$$UIManager$$get_Instance {};
        static inline StaticILMethod<0x04c676e0, Dpr::Message::MessageManager> Method$$MessageManager$$get_Instance {};
        static inline StaticILMethod<0x04c90318, SmartPoint::AssetAssistant::Sequencer> Method$$Sequencer$$get_Instance {};

        template <typename T>
        static inline typename T::Object* get_Instance(ILMethod<T>& method) {
            return external<typename T::Object*>(0x02322450, *method);
        }
    };
}
