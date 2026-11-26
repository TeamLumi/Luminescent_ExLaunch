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

    struct Component : ILClass<Component, game::Address::UnityEngine_Component_TypeInfo> {
        struct Fields : public UnityEngine::_Object::Fields {

        };

        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponent_SelectLanguageItem, Dpr::UI::SelectLanguageItem> Method$$SelectLanguageItem$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponent_SettingMenuItem, Dpr::UI::SettingMenuItem> Method$$SettingMenuItem$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponent_UIText, Dpr::UI::UIText> Method$$UIText$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponent_TextMeshProUGUI, TMPro::TextMeshProUGUI> Method$$TextMeshProUGUI$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponent_HorizontalLayoutGroup, UnityEngine::UI::HorizontalLayoutGroup> Method$$HorizontalLayoutGroup$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponent_Image, UnityEngine::UI::Image> Method$$Image$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponent_RectTransform, UnityEngine::RectTransform> Method$$RectTransform$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_GameObject_GetComponent_BoxCollider, UnityEngine::BoxCollider> Method$$BoxCollider$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_GameObject_GetComponent_BattleCharacterEntity, BattleCharacterEntity> Method$$BattleCharacterEntity$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_GameObject_GetComponent_FieldCharacterEntity, FieldCharacterEntity> Method$$FieldCharacterEntity$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponent_FieldObjectEntity, FieldObjectEntity> Method$$FieldObjectEntity$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponent_PokemonCustomNodeAnim, PokemonCustomNodeAnim> Method$$PokemonCustomNodeAnim$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponent_XMenuTopItem, XMenuTopItem> Method$$XMenuTopItem$$GetComponent {};

        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponentInChildren_UIText, Dpr::UI::UIText> Method$$UIText$$GetComponentInChildren {};

        static inline StaticILMethod<game::Address::Method_UnityEngine_Component_GetComponents_Component, Component> Method$$Component$$GetComponents {};

        template <typename T>
        inline T::Object* GetComponent() {
            return GetComponent(T::getClass());
        }

        template <typename T>
        inline T::Object* GetComponent(T::Class* type) {
            System::RuntimeTypeHandle::Object handle {};
            handle.fields.value = &type->_1.byval_arg;
            return external<typename T::Object*>(game::Address::UnityEngine_Component_GetComponent, this, System::Type::GetTypeFromHandle(handle));
        }

        template <typename T>
        inline T::Object* GetComponent(ILMethod<T>& method) {
            return external<typename T::Object*>(game::Address::UnityEngine_Component_GetComponent_ABSAnimationComponent, this, *method);
        }

        template <typename T>
        inline T::Object* GetComponentInChildren(bool includeInactive, ILMethod<T>& method) {
            return external<typename T::Object*>(game::Address::UnityEngine_Component_GetComponentInChildren_Animator, this, includeInactive, *method);
        }

        inline void GetComponents(System::Type* type, System::Collections::Generic::List$$Component* results) {
            external<void>(game::Address::UnityEngine_Component_GetComponents, this, type, results);
        }

        template <typename T>
        inline void GetComponents(System::Collections::Generic::List$$Component* results, ILMethod<T>& method) {
            external<void>(game::Address::UnityEngine_Component_GetComponents_BaseInputModule, this, results, *method);
        }

        inline UnityEngine::Transform* get_transform() {
            return external<UnityEngine::Transform*>(game::Address::UnityEngine_Component_get_transform, this);
        }

        inline UnityEngine::GameObject* get_gameObject() {
            return external<UnityEngine::GameObject*>(game::Address::UnityEngine_Component_get_gameObject, this);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$Component : List<List$$Component, UnityEngine::Component> {

    };
}
