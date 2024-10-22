#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/System/Type.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/Transform.h"

namespace UnityEngine {
    struct SkinnedMeshRenderer;
    struct Image;

    struct GameObject : ILClass<GameObject> {
        struct Fields : UnityEngine::_Object::Fields {

        };

        static inline StaticILMethod<0x04c67678, UnityEngine::SkinnedMeshRenderer> Method$$SkinnedMeshRenderer$$GetComponentsInChildren {};

        static inline StaticILMethod<0x04c66980, UnityEngine::Image> Method$$Image$$GetComponent {};

        inline UnityEngine::Transform::Object* get_transform() {
            return external<UnityEngine::Transform::Object*>(0x026b18d0, this);
        }

        static inline GameObject::Object* Find(System::String::Object *name) {
            return external<GameObject::Object*>(0x026b1ea0, name);
        }

        inline void SetActive(bool value) {
            external<void>(0x026b19c0, this, value);
        }

        inline bool get_activeSelf() {
            return external<bool>(0x026b1a10, this);
        }

        template <typename T>
        inline T::Object* GetComponent() {
            return GetComponent(T::getClass());
        }

        template <typename T>
        inline T::Object* GetComponent(T::Class* type) {
            System::RuntimeTypeHandle::Object handle {};
            handle.fields.value = &type->_1.byval_arg;
            return external<typename T::Object*>(0x026a8240, this, System::Type::GetTypeFromHandle(handle));
        }

        template <typename T>
        inline T::Object* GetComponent(ILMethod<T>& method) {
            return external<typename T::Object*>(0x01f48980, this, *method);
        }

        template <typename T>
        inline T::Array* GetComponentsInChildren(bool includeInactive) {
            return GetComponentsInChildren(T::getClass(), includeInactive);
        }

        template <typename T>
        inline T::Array* GetComponentsInternal(T::Class* type, bool includeInactive) {
            System::RuntimeTypeHandle::Object handle {};
            handle.fields.value = &type->_1.byval_arg;
            return external<typename T::Array*>(0x026b1680, this, System::Type::GetTypeFromHandle(handle), true, true, includeInactive, false, nullptr);
        }

        template <typename T>
        inline T::Array* GetComponentsInChildren(bool includeInactive, ILMethod<T>& method) {
            return external<typename T::Array*>(0x02bed170, this, includeInactive, *method);
        }
    };
}
