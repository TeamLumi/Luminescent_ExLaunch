#pragma once

namespace UnityEngine {
    struct Vector2Int : ILStruct<Vector2Int> {
        struct Fields {
            int32_t m_X;
            int32_t m_Y;
        };

        static inline bool op_Equality(Vector2Int::Object lhs, Vector2Int::Object rhs) {
            return external<bool>(game::Address::UnityEngine_Vector2Int_op_Equality, lhs, rhs);
        }
    };
}
