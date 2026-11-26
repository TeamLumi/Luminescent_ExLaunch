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
                .fields = external<Quaternion::Fields>(game::Address::UnityEngine_Quaternion_get_identity)
            };
        }
    };
}
