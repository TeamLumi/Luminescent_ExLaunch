#pragma once

#include "externals/il2cpp-api.h"
#include <cstdint>

namespace System {
    PRIMITIVE(bool, Boolean, 0x04c55610)
    PRIMITIVE(uint8_t, Byte, 0x04c55628)
    PRIMITIVE(uint16_t, Char, 0x04c553c8)
    PRIMITIVE(int16_t, Int16, 0x04c557e8)
    PRIMITIVE(uint16_t, UInt16, 0x04c557f0)
    PRIMITIVE(int32_t, Int32, 0x04c55270)
    PRIMITIVE(uint32_t, UInt32, 0x04c557f8)
    PRIMITIVE(int64_t, Int64, 0x04c55288)
    PRIMITIVE(uint64_t, UInt64, 0x04c55800)
    PRIMITIVE(float, Single, 0x04c55808)

    PRIMITIVE_ARRAY(Boolean, 0x04c55920)
    PRIMITIVE_ARRAY(Byte, 0x04c552e0)
    PRIMITIVE_ARRAY(Char, 0x04c55340)
    PRIMITIVE_ARRAY(Int16, 0x04c56380)
    PRIMITIVE_ARRAY(UInt16, 0x04c559e8)
    PRIMITIVE_ARRAY(Int32, 0x04c553e0)
    PRIMITIVE_ARRAY(UInt32, 0x04c558c8)
    PRIMITIVE_ARRAY(Int64, 0x04c56388)
    PRIMITIVE_ARRAY(UInt64, 0x04c563a0)
    PRIMITIVE_ARRAY(Single, 0x04c56398)
}
