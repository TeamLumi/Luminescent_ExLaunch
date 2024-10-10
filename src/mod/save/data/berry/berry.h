#pragma once

#include "externals/DPData/KinomiGrow.h"
#include "externals/PlayerWork.h"

#include "logger/logger.h"

template <int32_t size>
struct BerrySaveData {

    DPData::KinomiGrow::Object items[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            items[i].fields = {
                .tagNo = 0,
                .harvestCount = 0,
                .minutes = 0,
                ._wet = 0,
                ._selfPlant = 0,
                ._padding0 = 0,
                ._padding1 = 0,
            };
        }
    }
};

void linkBerries(PlayerWork::Object* playerWork);
