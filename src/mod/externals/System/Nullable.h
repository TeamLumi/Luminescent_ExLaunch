#pragma once

#include "externals/il2cpp-api.h"

namespace System {
    template <typename T>
    struct NullablePrimitive : ILStruct<NullablePrimitive<T>> {
        struct Fields {
            T value;
            bool has_value;
        };
    };

    template <typename T>
    struct Nullable : ILStruct<Nullable<T>> {
        struct Fields {
            T::Object value;
            bool has_value;
        };
    };
}
