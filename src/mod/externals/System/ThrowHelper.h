#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Touch.h"

namespace System {
    struct ThrowHelper : ILClass<ThrowHelper> {
        static inline void ThrowArgumentOutOfRangeException() {
            external<void>(game::Address::System_ThrowHelper_ThrowArgumentOutOfRangeException);
        }
    };
}
