#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace System {
    struct String;
}

namespace System::Text {
    struct UnicodeEncoding : ILClass<UnicodeEncoding> {
        inline uint32_t GetByteCount(System::String* str) {
            return external<uint32_t>(game::Address::System_Text_UnicodeEncoding_GetByteCount, this, str);
        }

        inline int32_t GetBytes(System::String* str, int32_t charIndex, int32_t charCount, System::Byte_array* bytes, int32_t byteIndex) {
            return external<int32_t>(game::Address::System_Text_UnicodeEncoding_GetBytes, this, str, charIndex, charCount, bytes, byteIndex);
        }

        inline System::String* GetString(System::Byte_array* bytes, int32_t index, int32_t count) {
            return external<System::String*>(game::Address::System_Text_UnicodeEncoding_GetString, this, bytes, index, count);
        }
    };
}
