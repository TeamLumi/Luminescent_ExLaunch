#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace Dpr::Box {
    struct BoxWork : ILClass<BoxWork> {

        static inline void SetTrayName(System::String::Object* str, int32_t tray) {
            external<void>(game::Address::Dpr_Box_BoxWork_SetTrayName, str, tray);
        }
    };
}
