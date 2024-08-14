#pragma once

#include "externals/il2cpp.h"

namespace DPData {
    struct TR_BATTLE_DATA : ILStruct<TR_BATTLE_DATA, 0x04c64da8> {
        struct Fields {
            bool IsWin;
            bool IsBattleSearcher;
        };
    };
}
