#pragma once

#include "externals/il2cpp-api.h"
#include <cstdint>

namespace System {
    typedef bool Boolean;
    typedef uint8_t Byte;
    typedef uint16_t Char;
    typedef uint16_t UInt16;
    typedef int32_t Int32;
    typedef uint32_t UInt32;
    typedef int64_t Int64;
    typedef float Single;

    PRIMITIVE_ARRAY(Boolean, 0x04c55920)
    PRIMITIVE_ARRAY(Byte, 0x04c552e0)
    PRIMITIVE_ARRAY(Char, 0x04c55340)
    PRIMITIVE_ARRAY(UInt16, 0x04c559e8)
    PRIMITIVE_ARRAY(Int32, 0x04c553e0)
    PRIMITIVE_ARRAY(UInt32, 0x04c558c8)
    PRIMITIVE_ARRAY(Int64, 0x04c56388)
    PRIMITIVE_ARRAY(Single, 0x04c56398)
}
