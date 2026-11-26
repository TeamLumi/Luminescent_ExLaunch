#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct Vector3 : ILStruct<Vector3> {
        struct Fields {
            float x;
            float y;
            float z;
        };

        inline void ctor(float x, float y, float z) {
            external<void>(game::Address::UnityEngine_Vector3_ctor, this, x, y, z);
        }

        static inline bool op_Equality(Vector3::Object lhs, Vector3::Object rhs) {
            UnityEngine::Vector3::Fields lhsProxy = { .x = lhs.fields.x, .y = lhs.fields.y, .z = lhs.fields.z };
            UnityEngine::Vector3::Fields rhsProxy = { .x = rhs.fields.x, .y = rhs.fields.y, .z = rhs.fields.z };
            return external<bool>(game::Address::UnityEngine_Vector3_op_Equality, lhsProxy, rhsProxy);
        }

        static inline bool op_Inequality(Vector3::Object lhs, Vector3::Object rhs) {
            UnityEngine::Vector3::Fields lhsProxy = { .x = lhs.fields.x, .y = lhs.fields.y, .z = lhs.fields.z };
            UnityEngine::Vector3::Fields rhsProxy = { .x = rhs.fields.x, .y = rhs.fields.y, .z = rhs.fields.z };
            return external<bool>(game::Address::UnityEngine_Vector3_op_Inequality, lhsProxy, rhsProxy);
        }

        static inline Vector3::Object op_Subtraction(Vector3::Object a, Vector3::Object b) {
            UnityEngine::Vector3::Fields aProxy = { .x = a.fields.x, .y = a.fields.y, .z = a.fields.z };
            UnityEngine::Vector3::Fields bProxy = { .x = b.fields.x, .y = b.fields.y, .z = b.fields.z };
            return {
                .fields = external<Vector3::Fields>(game::Address::UnityEngine_Vector3_op_Subtraction, aProxy, bProxy)
            };
        }

        static inline Vector3::Object get_zero() {
            return {
                .fields = external<Vector3::Fields>(game::Address::UnityEngine_Vector3_get_zero)
            };
        }

        inline float get_magnitude() {
            return external<float>(game::Address::UnityEngine_Vector3_get_magnitude, this);
        }
    };
}