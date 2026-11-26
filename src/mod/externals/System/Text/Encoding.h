#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Text/UnicodeEncoding.h"
#include "externals/System/Text/UTF8Encoding.h"

namespace System::Text {
    struct Encoding : ILClass<Encoding> {
        static inline System::Text::UTF8Encoding::Object* get_UTF8() {
            return external<System::Text::UTF8Encoding::Object*>(game::Address::System_Text_Encoding_get_UTF8);
        }

        static inline System::Text::UnicodeEncoding::Object* get_Unicode() {
            return external<System::Text::UnicodeEncoding::Object*>(game::Address::System_Text_Encoding_get_Unicode);
        }
    };
}
