#pragma once


namespace UnityEngine {
    struct Quaternion : ILStruct<Quaternion> {
        struct Fields {
            float x;
            float y;
            float z;
            float w;
        };

        static inline Quaternion::Object get_identity() {
            return {
                .fields = external<Quaternion::Fields>(0x02692cf0)
            };
        }
    };
}
