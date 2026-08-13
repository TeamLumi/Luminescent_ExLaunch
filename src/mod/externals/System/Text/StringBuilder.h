#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"

namespace System::Text {
    struct StringBuilder : ILClass<StringBuilder> {
        struct Fields {
            // TODO
        };

        inline System::String::Object* ToString() {
            return external<System::String::Object*>(0x0286c0a0, this);
        }

        inline StringBuilder::Object* Clear() {
            return external<StringBuilder::Object*>(0x0286c450, this);
        }

        inline StringBuilder::Object* Append(System::String::Object* value) {
            return external<StringBuilder::Object*>(0x0285cd40, this, value);
        }

        inline int32_t get_Length() {
            return external<int32_t>(0x0285d7a0, this);
        }
    };
}
