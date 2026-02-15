#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/Vector3.h"

namespace UnityEngine {
    struct Transform : ILClass<Transform, 0x4c57e80> {
        struct Fields : UnityEngine::Component::Fields {

        };

        inline void SetParent(UnityEngine::Transform* parent, bool worldPositionStays) {
            external<void>(0x0299e340, this, parent, worldPositionStays);
        }

        inline void SetParent(UnityEngine::Transform* p) {
            external<void>(0x0299e2e0, this, p);
        }

        inline void SetSiblingIndex(int32_t index) {
            external<void>(0x0299f400, this, index);
        }

        inline int32_t get_childCount() {
            return external<int32_t>(0x0299f2c0, this);
        }

        inline UnityEngine::Transform::Object* GetParent() {
            return external<UnityEngine::Transform::Object*>(0x0299e290, this);
        }

        inline UnityEngine::Transform::Object* GetChild(int32_t index) {
            return external<UnityEngine::Transform::Object*>(0x0299f7d0, this, index);
        }

        inline UnityEngine::Transform::Object* Find(System::String* name) {
            return external<UnityEngine::Transform::Object*>(0x0299f500, this, name);
        }
        
        inline void Translate(UnityEngine::Vector3* translation) {
            external<void>(0x0299e630, this, translation);
        }

        inline UnityEngine::Vector3::Object get_position() {
            return {
                .fields = external<UnityEngine::Vector3::Fields>(0x0299d1c0, this)
            };
        }

        inline void set_position(UnityEngine::Vector3::Object value) {
            UnityEngine::Vector3::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y, .z = value.fields.z };
            external<void>(0x0299d270, this, valueProxy);
        }

        inline UnityEngine::Vector3::Object get_localPosition() {
            return {
                .fields = external<UnityEngine::Vector3::Fields>(0x0299d320, this)
            };
        }

        inline void set_localPosition(UnityEngine::Vector3::Object value) {
            UnityEngine::Vector3::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y, .z = value.fields.z };
            external<void>(0x0299d3d0, this, valueProxy);
        }

        inline UnityEngine::Vector3::Object get_localEulerAngles() {
            return {
                .fields = external<UnityEngine::Vector3::Fields>(0x0299d690, this)
            };
        }

        inline void set_localEulerAngles(UnityEngine::Vector3::Object value) {
            UnityEngine::Vector3::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y, .z = value.fields.z };
            external<void>(0x0299d770, this, valueProxy);
        }

        inline UnityEngine::Vector3::Object get_localScale() {
            return {
                .fields = external<UnityEngine::Vector3::Fields>(0x0299df50, this)
            };
        }

        inline void set_localScale(UnityEngine::Vector3::Object value) {
            UnityEngine::Vector3::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y, .z = value.fields.z };
            external<void>(0x0299e000, this, valueProxy);
        }

        inline UnityEngine::Vector2::Object get_sizeDelta() {
            return {
                .fields = external<UnityEngine::Vector2::Fields>(0x02696610, this)
            };
        }

        inline void RotateAround(UnityEngine::Vector3::Object point, UnityEngine::Vector3::Object axis, float angle) {
            UnityEngine::Vector3::Fields pointProxy = { .x = point.fields.x, .y = point.fields.y, .z = point.fields.z };
            UnityEngine::Vector3::Fields axisProxy = { .x = axis.fields.x, .y = axis.fields.y, .z = axis.fields.z };
            external<void>(0x0299ec40, this, pointProxy, axisProxy, angle);
        }

        // utility functions
        UnityEngine::Transform::Object* GetChild(std::initializer_list<std::int32_t> index) {
            UnityEngine::Transform* transform = this;
            for (int32_t i : index) {
                transform = transform->GetChild(i);
            }
            return transform->instance();
        }

        template <typename T>
        inline T::Object* GetComponent() {
            return this->cast<UnityEngine::Component>()->GetComponent<T>();
        }

        template <typename T>
        inline T::Object* GetComponent(ILMethod<T>& method) {
            return this->cast<UnityEngine::Component>()->GetComponent<T>(method);
        }

        template <typename T>
        inline T::Object* GetComponent(T::Class* klass) {
            return this->cast<UnityEngine::Component>()->GetComponent<T>(klass);
        }

        template <typename T>
        inline void GetComponents(System::Collections::Generic::List$$Component* results, ILMethod<T>& method) {
            this->cast<UnityEngine::Component>()->GetComponents<T>(results, method);
        }
    };
}
