#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector2.h"

namespace UnityEngine {
    struct RectTransform : ILClass<RectTransform> {
        struct Fields : UnityEngine::Transform::Fields {

        };

        inline UnityEngine::Vector2::Object get_sizeDelta() {
            return {
                    .fields = external<UnityEngine::Vector2::Fields>(0x02696610, this)
            };
        }

        inline void set_sizeDelta(UnityEngine::Vector2::Object value) {
            UnityEngine::Vector2::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y };
            external<void>(0x026966c0, this, valueProxy);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$RectTransform : List<List$$RectTransform, UnityEngine::RectTransform> {
        static inline StaticILMethod<0x04c8ad78, UnityEngine::RectTransform> Method$$Add {};
    };
}
