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
                    .fields = external<UnityEngine::Vector2::Fields>(game::Address::UnityEngine_RectTransform_get_sizeDelta, this)
            };
        }

        inline void set_sizeDelta(UnityEngine::Vector2::Object value) {
            UnityEngine::Vector2::Fields valueProxy = { .x = value.fields.x, .y = value.fields.y };
            external<void>(game::Address::UnityEngine_RectTransform_set_sizeDelta, this, valueProxy);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$RectTransform : List<List$$RectTransform, UnityEngine::RectTransform> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_RectTransform_Add, UnityEngine::RectTransform> Method$$Add {};
    };
}
