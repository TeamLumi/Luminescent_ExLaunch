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
    struct SingletonMonoBehaviour : ILClass<SingletonMonoBehaviour, game::Address::SmartPoint_AssetAssistant_SingletonMonoBehaviour_UIManager_TypeInfo> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {};

        static inline StaticILMethod<game::Address::Method_SmartPoint_AssetAssistant_SingletonMonoBehaviour_AudioManager_get_Instance, Audio::AudioManager> Method$$AudioManager$$get_Instance {};
        static inline StaticILMethod<game::Address::Method_SmartPoint_AssetAssistant_SingletonMonoBehaviour_UIManager_get_Instance, Dpr::UI::UIManager> Method$$UIManager$$get_Instance {};
        static inline StaticILMethod<game::Address::Method_SmartPoint_AssetAssistant_SingletonMonoBehaviour_MessageManager_get_Instance, Dpr::Message::MessageManager> Method$$MessageManager$$get_Instance {};

        template <typename T>
        static inline typename T::Object* get_Instance(ILMethod<T>& method) {
            return external<typename T::Object*>(game::Address::SmartPoint_AssetAssistant_SingletonMonoBehaviour_AudioManager_get_Instance, *method);
        }
    };
}
