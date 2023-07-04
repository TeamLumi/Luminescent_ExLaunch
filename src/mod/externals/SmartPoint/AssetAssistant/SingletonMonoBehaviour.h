#pragma once

#include "externals/il2cpp.h"

#include "externals/UnityEngine/MonoBehaviour.h"

namespace Audio {
    struct AudioManager;
}

namespace Dpr {
    namespace Battle {
        namespace View {
            struct BattleViewCore;
        }
    }

    namespace UI {
        struct UIManager;
    }
}

namespace SmartPoint {
    namespace Components {
        struct DebugMenu;
    }
}

namespace SmartPoint::AssetAssistant {
    struct SingletonMonoBehaviour : ILClass<SingletonMonoBehaviour, 0x04c59b28> {
        struct Fields : public UnityEngine::MonoBehaviour::Fields {

        };

        static_assert(offsetof(Fields, m_CachedPtr) == 0);

        static inline StaticILMethod<0x04c676a0, Audio::AudioManager> Method$$AudioManager$$get_Instance {};
        static inline StaticILMethod<0x04c676c0, Dpr::Battle::View::BattleViewCore> Method$$BattleViewCore$$get_Instance {};
        static inline StaticILMethod<0x04c67700, Dpr::UI::UIManager> Method$$UIManager$$get_Instance {};
        static inline StaticILMethod<0x04c67720, SmartPoint::Components::DebugMenu> Method$$DebugMenu$$get_Instance {};

        template <typename T>
        static inline typename T::Object* get_Instance(ILMethod<T>& method) {
            return external<typename T::Object*>(0x02322450, *method);
        }
    };
}
