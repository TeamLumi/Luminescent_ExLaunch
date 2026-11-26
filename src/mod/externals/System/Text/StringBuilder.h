#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"

namespace System::Text {
    struct StringBuilder : ILClass<StringBuilder> {
        struct Fields {
            // TODO
        };

        inline System::String::Object* ToString() {
            return external<System::String::Object*>(game::Address::System_Text_StringBuilder_ToString, this);
        }

        inline StringBuilder::Object* Clear() {
            return external<StringBuilder::Object*>(game::Address::System_Text_StringBuilder_Clear, this);
        }

        inline StringBuilder::Object* Append(System::String::Object* value) {
            return external<StringBuilder::Object*>(game::Address::System_Text_StringBuilder_Append_String, this, value);
        }

        inline int32_t get_Length() {
            return external<int32_t>(game::Address::System_Text_StringBuilder_get_Length, this);
        }
    };
}
