#pragma once

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"

#include "logger/logger.h"

template <int32_t size>
struct FlagSaveData {

    System::Boolean items[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            items[i] = false;
        }
    }
};

void linkFlags(PlayerWork::Object* playerWork);