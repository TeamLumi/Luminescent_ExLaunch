#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace System {
    struct String;
}

namespace System::Text {
    struct UTF8Encoding : ILClass<UTF8Encoding> {
        inline uint32_t GetByteCount(System::String* str) {
            return external<uint32_t>(game::Address::System_Text_UTF8Encoding_GetByteCount, this, str);
        }

        inline int32_t GetBytes(System::String* str, int32_t charIndex, int32_t charCount, System::Byte_array* bytes, int32_t byteIndex) {
            return external<int32_t>(game::Address::System_Text_UTF8Encoding_GetBytes, this, str, charIndex, charCount, bytes, byteIndex);
        }
    };
}
