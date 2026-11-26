#pragma once

#include "externals/il2cpp.h"

namespace DPData {
    struct TR_BATTLE_DATA : ILStruct<TR_BATTLE_DATA, game::Address::DPData_TR_BATTLE_DATA_Array_TypeInfo> {
        struct Fields {
            bool IsWin;
            bool IsBattleSearcher;
        };
    };
}
