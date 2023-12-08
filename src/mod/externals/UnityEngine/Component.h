#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/_Object.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/System/Type.h"
#include "externals/System/_Object.h"

// These are all to avoid cyclical definitions
struct BattleCharacterEntity;
struct FieldCharacterEntity;

namespace System::Collections::Generic {
    struct List$$Component;
}

namespace UnityEngine::UI {
    struct HorizontalLayoutGroup;
}

namespace Dpr {
    namespace Battle::View::UI {
        struct BUISituation;
    }

    namespace UI {
        struct SelectLanguageItem;
        struct SettingMenuItem;
        struct UIText;
    }
}

namespace UnityEngine {
    struct Transform;
    struct RectTransform;
    struct BoxCollider;
    struct GameObject;

    struct Component : ILClass<Component, 0x04c57e88> {
        struct Fields : public UnityEngine::_Object::Fields {

        };

        static inline StaticILMethod<0x04c667c0, Dpr::UI::SelectLanguageItem> Method$$SelectLanguageItem$$GetComponent {};
        static inline StaticILMethod<0x04c667d0, Dpr::UI::SettingMenuItem> Method$$SettingMenuItem$$GetComponent {};
        static inline StaticILMethod<0x04c667e0, Dpr::UI::UIText> Method$$UIText$$GetComponent {};
        static inline StaticILMethod<0x04c66970, UnityEngine::UI::HorizontalLayoutGroup> Method$$HorizontalLayoutGroup$$GetComponent {};
        static inline StaticILMethod<0x04c66918, UnityEngine::RectTransform> Method$$RectTransform$$GetComponent {};
        static inline StaticILMethod<0x04c67050, UnityEngine::BoxCollider> Method$$BoxCollider$$GetComponent {};
        static inline StaticILMethod<0x04c66d60, BattleCharacterEntity> Method$$BattleCharacterEntity$$GetComponent {};
        static inline StaticILMethod<0x04c66fc0, FieldCharacterEntity> Method$$FieldCharacterEntity$$GetComponent {};

        template <typename T>
        inline T::Object* GetComponent() {
            return GetComponent(T::getClass());
        }

        template <typename T>
        inline T::Object* GetComponent(T::Class* type) {
            System::RuntimeTypeHandle::Object handle {};
            handle.fields.value = &type->_1.byval_arg;
            return external<typename T::Object*>(0x026a81c0, this, System::Type::GetTypeFromHandle(handle));
        }

        template <typename T>
        inline T::Object* GetComponent(ILMethod<T>& method) {
            return external<typename T::Object*>(0x01f48340, this, *method);
        }

        inline void GetComponents(System::Type* type, System::Collections::Generic::List$$Component* results) {
            external<void>(0x026a8510, this, type, results);
        }

        inline UnityEngine::Transform* get_transform() {
            return external<UnityEngine::Transform*>(0x026a8120, this);
        }

        inline UnityEngine::GameObject* get_gameObject() {
            return external<UnityEngine::GameObject*>(0x026a8170, this);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$Component : List<List$$Component, UnityEngine::Component> {

    };
}
