#pragma once

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"

#include "logger/logger.h"

template <int32_t size>
struct WorkSaveData {

    System::Int32 items[size];

    void Initialize() {
        for (int32_t i = 0; i < size; i++) {
            items[i] = 0;
        }
    }
};

void linkWorks(PlayerWork::Object* playerWork);
