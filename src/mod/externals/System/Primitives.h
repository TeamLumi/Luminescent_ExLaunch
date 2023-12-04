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

    PRIMITIVE_ARRAY(Boolean);
    static Il2CppClass* Boolean_array_TypeInfo() {
        return *reinterpret_cast<Il2CppClass**>(exl::util::modules::GetTargetOffset(0x04c55920));
    }
    PRIMITIVE_ARRAY(Byte);
    static Il2CppClass* Byte_array_TypeInfo() {
        return *reinterpret_cast<Il2CppClass**>(exl::util::modules::GetTargetOffset(0x04c552e0));
    }
    PRIMITIVE_ARRAY(Char);
    PRIMITIVE_ARRAY(UInt16);
    PRIMITIVE_ARRAY(Int32);
    static Il2CppClass* Int32_array_TypeInfo() {
        return *reinterpret_cast<Il2CppClass**>(exl::util::modules::GetTargetOffset(0x04c553e0));
    }
    PRIMITIVE_ARRAY(UInt32);
    PRIMITIVE_ARRAY(Int64);
    PRIMITIVE_ARRAY(Single);
}
