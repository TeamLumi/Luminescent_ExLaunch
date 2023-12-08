#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::View::Systems {
    struct MsgShowParam : ILStruct<MsgShowParam> {
        struct Fields {
            int32_t MsgAfterglow;
            bool IsKeyWait;
        };
    };
}
