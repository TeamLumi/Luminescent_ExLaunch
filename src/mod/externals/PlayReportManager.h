#pragma once

#include "externals/il2cpp-api.h"

struct PlayReportManager : ILStruct<PlayReportManager, game::Address::PlayReportManager_TypeInfo> {
    struct Fields {
    };

    static inline void StartWildBattle(int32_t type) {
        external<void>(game::Address::PlayReportManager_StartWildBattle, type);
    }
};
