#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct Vector2 : ILStruct<Vector2> {
        struct Fields {
            float x;
            float y;
        };

        inline void ctor(float x, float y) {
            external<void>(game::Address::UnityEngine_Vector2_ctor, x, y);
        }

        static inline Vector2::Object get_zero() {
            return {
                .fields = external<Vector2::Fields>(game::Address::UnityEngine_Vector2_get_zero)
            };
        }

        static inline bool op_Equality(Vector2::Object lhs, Vector2::Object rhs) {
            UnityEngine::Vector2::Fields lhsProxy = { .x = lhs.fields.x, .y = lhs.fields.y };
            UnityEngine::Vector2::Fields rhsProxy = { .x = rhs.fields.x, .y = rhs.fields.y };
            return external<bool>(game::Address::UnityEngine_Vector2_op_Equality, lhsProxy, rhsProxy);
        }

        static inline bool op_Inequality(Vector2::Object lhs, Vector2::Object rhs) {
            UnityEngine::Vector2::Fields lhsProxy = { .x = lhs.fields.x, .y = lhs.fields.y };
            UnityEngine::Vector2::Fields rhsProxy = { .x = rhs.fields.x, .y = rhs.fields.y };
            return external<bool>(game::Address::UnityEngine_Vector2_op_Inequality, lhsProxy, rhsProxy);
        }
    };
}

static_assert(sizeof(UnityEngine::Vector2::Object) == 0x8);
