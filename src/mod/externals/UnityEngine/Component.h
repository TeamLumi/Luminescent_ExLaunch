#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/_Object.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/System/Type.h"
#include "externals/System/_Object.h"

// These are all to avoid cyclical definitions
struct BattleCharacterEntity;
struct FieldCharacterEntity;
struct FieldObjectEntity;
struct PokemonCustomNodeAnim;

namespace System::Collections::Generic {
    struct List$$Component;
}

namespace UnityEngine::UI {
    struct HorizontalLayoutGroup;
}

namespace Dpr::UI {
    struct SelectLanguageItem;
    struct SettingMenuItem;
    struct UIText;
}

namespace TMPro {
    struct TextMeshProUGUI;
}

namespace UnityEngine {
    struct Transform;
    struct RectTransform;
    struct BoxCollider;
    struct GameObject;
    struct XMenuTopItem;

    namespace UI {
        struct Image;
    }

    struct Component : ILClass<Component, 0x04c57e88> {
        struct Fields : public UnityEngine::_Object::Fields {

        };

        static inline StaticILMethod<0x04c667c0, Dpr::UI::SelectLanguageItem> Method$$SelectLanguageItem$$GetComponent {};
        static inline StaticILMethod<0x04c667d0, Dpr::UI::SettingMenuItem> Method$$SettingMenuItem$$GetComponent {};
        static inline StaticILMethod<0x04c667e0, Dpr::UI::UIText> Method$$UIText$$GetComponent {};
        static inline StaticILMethod<0x04c66890, TMPro::TextMeshProUGUI> Method$$TextMeshProUGUI$$GetComponent {};
        static inline StaticILMethod<0x04c66970, UnityEngine::UI::HorizontalLayoutGroup> Method$$HorizontalLayoutGroup$$GetComponent {};
        static inline StaticILMethod<0x04c66980, UnityEngine::UI::Image> Method$$Image$$GetComponent {};
        static inline StaticILMethod<0x04c66918, UnityEngine::RectTransform> Method$$RectTransform$$GetComponent {};
        static inline StaticILMethod<0x04c67050, UnityEngine::BoxCollider> Method$$BoxCollider$$GetComponent {};
        static inline StaticILMethod<0x04c66d60, BattleCharacterEntity> Method$$BattleCharacterEntity$$GetComponent {};
        static inline StaticILMethod<0x04c66fc0, FieldCharacterEntity> Method$$FieldCharacterEntity$$GetComponent {};
        static inline StaticILMethod<0x04c66808, FieldObjectEntity> Method$$FieldObjectEntity$$GetComponent {};
        static inline StaticILMethod<0x04c66840, PokemonCustomNodeAnim> Method$$PokemonCustomNodeAnim$$GetComponent {};
        static inline StaticILMethod<0x04c669c0, XMenuTopItem> Method$$XMenuTopItem$$GetComponent {};

        static inline StaticILMethod<0x04c66a18, Dpr::UI::UIText> Method$$UIText$$GetComponentInChildren {};

        static inline StaticILMethod<0x04c8e360, Component> Method$$Component$$GetComponents {};

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

        template <typename T>
        inline T::Object* GetComponentInChildren(bool includeInactive, ILMethod<T>& method) {
            return external<typename T::Object*>(0x01f484e0, this, includeInactive, *method);
        }

        inline void GetComponents(System::Type* type, System::Collections::Generic::List$$Component* results) {
            external<void>(0x026a8510, this, type, results);
        }

        template <typename T>
        inline void GetComponents(System::Collections::Generic::List$$Component* results, ILMethod<T>& method) {
            external<void>(0x01cf27c0, this, results, *method);
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
