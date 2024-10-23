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

    static Il2CppClass* ValueTuple2$$String$$int32_array_TypeInfo() {
        return *reinterpret_cast<Il2CppClass**>(exl::util::modules::GetTargetOffset(0x04c5dce0));
    }
}
