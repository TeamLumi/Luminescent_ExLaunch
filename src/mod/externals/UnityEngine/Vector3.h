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
            external<void>(0x0299e850, this, x, y, z);
        }

        static inline bool op_Equality(Vector3::Object lhs, Vector3::Object rhs) {
            UnityEngine::Vector3::Fields lhsProxy = { .x = lhs.fields.x, .y = lhs.fields.y, .z = lhs.fields.z };
            UnityEngine::Vector3::Fields rhsProxy = { .x = rhs.fields.x, .y = rhs.fields.y, .z = rhs.fields.z };
            return external<bool>(0x029a3d90, lhsProxy, rhsProxy);
        }

        static inline bool op_Inequality(Vector3::Object lhs, Vector3::Object rhs) {
            UnityEngine::Vector3::Fields lhsProxy = { .x = lhs.fields.x, .y = lhs.fields.y, .z = lhs.fields.z };
            UnityEngine::Vector3::Fields rhsProxy = { .x = rhs.fields.x, .y = rhs.fields.y, .z = rhs.fields.z };
            return external<bool>(0x029a3dd0, lhsProxy, rhsProxy);
        }

        static inline Vector3::Object op_Multiply(Vector3::Object a, float d) {
            UnityEngine::Vector3::Fields aProxy = { .x = a.fields.x, .y = a.fields.y, .z = a.fields.z };
            return {
                .fields = external<Vector3::Fields>(0x029a3d60, aProxy, d)
            };
        }

        static inline Vector3::Object get_zero() {
            return {
                .fields = external<Vector3::Fields>(0x029a3170)
            };
        }
    };
}
