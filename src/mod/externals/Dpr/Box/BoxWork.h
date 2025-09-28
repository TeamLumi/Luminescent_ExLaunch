#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace Dpr::Box {
    struct BoxWork : ILClass<BoxWork> {

        static inline void SetTrayName(System::String::Object* str, int32_t tray) {
            external<void>(0x01d322a0, str, tray);
        }
    };
}
