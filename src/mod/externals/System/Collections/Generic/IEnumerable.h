#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace System::Collections::Generic {
    template <typename T, typename E>
    struct IEnumerable : ILClass<T> {
        struct Fields {};  // Empty for some reason?
    };
}
