#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/System/Type.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/Transform.h"

namespace Dpr {
    struct PatcheelPattern;
}

namespace UnityEngine {
    namespace UI {
        struct Image;
    }

    struct SkinnedMeshRenderer;

    struct GameObject : ILClass<GameObject, game::Address::UnityEngine_GameObject_TypeInfo> {
        struct Fields : UnityEngine::_Object::Fields {

        };

        static inline StaticILMethod<game::Address::Method_UnityEngine_GameObject_GetComponentsInChildren_SkinnedMeshRenderer, UnityEngine::SkinnedMeshRenderer> Method$$SkinnedMeshRenderer$$GetComponentsInChildren {};

        static inline StaticILMethod<game::Address::Method_UnityEngine_GameObject_GetComponent_PatcheelPattern, Dpr::PatcheelPattern> Method$$PatcheelPattern$$GetComponent {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_GameObject_GetComponent_Image, UnityEngine::UI::Image>   Method$$Image$$GetComponent {};

        static inline StaticILMethod<game::Address::Method_UnityEngine_GameObject_GetComponents_Component, UnityEngine::Component> Method$$Component$$GetComponents {};

        inline void ctor() {
            external<void>(game::Address::UnityEngine_GameObject_ctor_NoArgs, this);
        }

        inline UnityEngine::Transform::Object* get_transform() {
            return external<UnityEngine::Transform::Object*>(game::Address::UnityEngine_GameObject_get_transform, this);
        }

        static inline GameObject::Object* Find(System::String::Object *name) {
            return external<GameObject::Object*>(game::Address::UnityEngine_GameObject_Find, name);
        }

        inline void SetActive(bool value) {
            external<void>(game::Address::UnityEngine_GameObject_SetActive, this, value);
        }

        inline bool get_activeSelf() {
            return external<bool>(game::Address::UnityEngine_GameObject_get_activeSelf, this);
        }

        template <typename T>
        inline T::Array* GetComponentsInternal(System::Type::Object* type, bool useSearchTypeAsArrayReturnType, bool recursive, bool includeInactive, bool reverse, Il2CppObject* resultList) {
            return external<typename T::Array*>(game::Address::UnityEngine_GameObject_GetComponentsInternal, this, type, useSearchTypeAsArrayReturnType, recursive, includeInactive, reverse, resultList);
        }

        inline Component::Array* GetAllComponents() {
            System::RuntimeTypeHandle::Object handle {};
            handle.fields.value = &Component::getClass()->_1.byval_arg;
            return GetComponentsInternal<Component>(System::Type::GetTypeFromHandle(handle), true, false, true, false, nullptr);
        }

        template <typename T>
        inline T::Object* GetComponent() {
            return GetComponent(T::getClass());
        }

        template <typename T>
        inline T::Object* GetComponent(T::Class* type) {
            System::RuntimeTypeHandle::Object handle {};
            handle.fields.value = &type->_1.byval_arg;
            return external<typename T::Object*>(game::Address::UnityEngine_GameObject_GetComponent, this, System::Type::GetTypeFromHandle(handle));
        }

        template <typename T>
        inline T::Object* GetComponent(ILMethod<T>& method) {
            return external<typename T::Object*>(game::Address::UnityEngine_GameObject_GetComponent_AkGameObj, this, *method);
        }

        template <typename T>
        inline T::Array* GetComponentsInChildren(bool includeInactive) {
            return GetComponentsInChildren(T::getClass(), includeInactive);
        }

        template <typename T>
        inline T::Array* GetComponentsInternal(T::Class* type, bool includeInactive) {
            System::RuntimeTypeHandle::Object handle {};
            handle.fields.value = &type->_1.byval_arg;
            return external<typename T::Array*>(game::Address::UnityEngine_GameObject_GetComponentsInternal, this, System::Type::GetTypeFromHandle(handle), true, true, includeInactive, false, nullptr);
        }

        template <typename T>
        inline T::Array* GetComponentsInChildren(bool includeInactive, ILMethod<T>& method) {
            return external<typename T::Array*>(game::Address::UnityEngine_GameObject_GetComponentsInChildren_EffectActivator, this, includeInactive, *method);
        }
    };
}
