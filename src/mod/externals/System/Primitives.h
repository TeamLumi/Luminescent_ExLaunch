#pragma once

#include "externals/il2cpp-api.h"
#include <cstdint>

namespace System {
    PRIMITIVE(bool, Boolean, game::Address::bool_TypeInfo)
    PRIMITIVE(uint8_t, Byte, game::Address::byte_TypeInfo)
    PRIMITIVE(uint16_t, Char, game::Address::char_TypeInfo)
    PRIMITIVE(int16_t, Int16, game::Address::short_TypeInfo)
    PRIMITIVE(uint16_t, UInt16, game::Address::ushort_TypeInfo)
    PRIMITIVE(int32_t, Int32, game::Address::int_TypeInfo)
    PRIMITIVE(uint32_t, UInt32, game::Address::uint_TypeInfo)
    PRIMITIVE(int64_t, Int64, game::Address::long_TypeInfo)
    PRIMITIVE(uint64_t, UInt64, game::Address::ulong_TypeInfo)
    PRIMITIVE(float, Single, game::Address::float_TypeInfo)

    PRIMITIVE_ARRAY(Boolean, game::Address::bool_Array_TypeInfo)
    PRIMITIVE_ARRAY(Byte, game::Address::byte_Array_TypeInfo)
    PRIMITIVE_ARRAY(Char, game::Address::char_Array_TypeInfo)
    PRIMITIVE_ARRAY(Int16, game::Address::short_Array_TypeInfo)
    PRIMITIVE_ARRAY(UInt16, game::Address::ushort_Array_TypeInfo)
    PRIMITIVE_ARRAY(Int32, game::Address::int_Array_TypeInfo)
    PRIMITIVE_ARRAY(UInt32, game::Address::uint_Array_TypeInfo)
    PRIMITIVE_ARRAY(Int64, game::Address::long_Array_TypeInfo)
    PRIMITIVE_ARRAY(UInt64, game::Address::ulong_Array_TypeInfo)
    PRIMITIVE_ARRAY(Single, game::Address::float_Array_TypeInfo)
}
