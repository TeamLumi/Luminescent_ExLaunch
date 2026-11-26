#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/Vector3.h"

namespace UnityEngine {
    struct Transform : ILClass<Transform, game::Address::UnityEngine_Transform_TypeInfo> {
        struct Fields : UnityEngine::Component::Fields {

        };

        inline void SetParent(UnityEngine::Transform* parent, bool worldPositionStays) {
            external<void>(game::Address::UnityEngine_Transform_SetParent_Transform_Bool, this, parent, worldPositionStays);
        }

        inline void SetParent(UnityEngine::Transform* p) {
            external<void>(game::Address::UnityEngine_Transform_SetParent_Transform, this, p);
        }

        inline void SetSiblingIndex(int32_t index) {
            external<void>(game::Address::UnityEngine_Transform_SetSiblingIndex, this, index);
        }

        inline int32_t get_childCount() {
            return external<int32_t>(game::Address::UnityEngine_Transform_get_childCount, this);
        }

        inline UnityEngine::Transform::Object* GetParent() {
            return external<UnityEngine::Transform::Object*>(game::Address::UnityEngine_Transform_GetParent, this);
        }

        inline UnityEngine::Transform::Object* GetChild(int32_t index) {
            return external<UnityEngine::Transform::Object*>(game::Address::UnityEngine_Transform_GetChild, this, index);
        }

        inline UnityEngine::Transform::Object* Find(System::String* name) {
            return external<UnityEngine::Transform::Object*>(game::Address::UnityEngine_Transform_Find, this, name);
        }
        
        inline void Translate(UnityEngine::Vector3* translation) {
            external<void>(game::Address::UnityEngine_Transform_Translate, this, translation);
        }

        inline UnityEngine::Vector3::Object get_position() {
            return {
                .fields = external<UnityEngine::Vector3::Fields>(game::Address::UnityEngine_Transform_get_position, this)
            };
        }

        inline void set_position(UnityEngine::Vector3::Object value) {
            UnityEngine::Vector3::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y, .z = value.fields.z };
            external<void>(game::Address::UnityEngine_Transform_set_position, this, valueProxy);
        }

        inline UnityEngine::Vector3::Object get_localPosition() {
            return {
                .fields = external<UnityEngine::Vector3::Fields>(game::Address::UnityEngine_Transform_get_localPosition, this)
            };
        }

        inline void set_localPosition(UnityEngine::Vector3::Object value) {
            UnityEngine::Vector3::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y, .z = value.fields.z };
            external<void>(game::Address::UnityEngine_Transform_set_localPosition, this, valueProxy);
        }

        inline UnityEngine::Vector3::Object get_localEulerAngles() {
            return {
                .fields = external<UnityEngine::Vector3::Fields>(game::Address::UnityEngine_Transform_get_localEulerAngles, this)
            };
        }

        inline void set_localEulerAngles(UnityEngine::Vector3::Object value) {
            UnityEngine::Vector3::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y, .z = value.fields.z };
            external<void>(game::Address::UnityEngine_Transform_set_localEulerAngles, this, valueProxy);
        }

        inline UnityEngine::Vector3::Object get_localScale() {
            return {
                .fields = external<UnityEngine::Vector3::Fields>(game::Address::UnityEngine_Transform_get_localScale, this)
            };
        }

        inline void set_localScale(UnityEngine::Vector3::Object value) {
            UnityEngine::Vector3::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y, .z = value.fields.z };
            external<void>(game::Address::UnityEngine_Transform_set_localScale, this, valueProxy);
        }

        inline UnityEngine::Vector2::Object get_sizeDelta() {
            return {
                .fields = external<UnityEngine::Vector2::Fields>(game::Address::UnityEngine_RectTransform_get_sizeDelta, this)
            };
        }

        inline void RotateAround(UnityEngine::Vector3::Object point, UnityEngine::Vector3::Object axis, float angle) {
            UnityEngine::Vector3::Fields pointProxy = { .x = point.fields.x, .y = point.fields.y, .z = point.fields.z };
            UnityEngine::Vector3::Fields axisProxy = { .x = axis.fields.x, .y = axis.fields.y, .z = axis.fields.z };
            external<void>(game::Address::UnityEngine_Transform_RotateAround, this, pointProxy, axisProxy, angle);
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
