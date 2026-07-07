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
    };
}
