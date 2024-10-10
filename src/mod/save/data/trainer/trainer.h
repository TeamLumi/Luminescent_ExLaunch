#pragma once

#include "externals/DPData/TR_BATTLE_DATA.h"
#include "externals/PlayerWork.h"

#include "logger/logger.h"

template <int32_t size>
struct TrainerSaveData {

    DPData::TR_BATTLE_DATA::Object items[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            items[i].fields = {
                .IsWin = false,
                .IsBattleSearcher = false,
            };
        }
    }
};

void linkTrainers(PlayerWork::Object* playerWork);