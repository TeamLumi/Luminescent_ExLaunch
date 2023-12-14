#pragma once

#include "externals/il2cpp-api.h"

namespace System {
    template <typename T, typename I1, typename I2>
    struct ValueTuple2 : ILStruct<T> {
        struct Fields {
            I1 Item1;
            I2 Item2;
        };
    };

    template <typename T, typename I1, typename I2, typename I3>
    struct ValueTuple3 : ILStruct<T> {
        struct Fields {
            I1 Item1;
            I2 Item2;
            I3 Item3;
        };
    };
}
