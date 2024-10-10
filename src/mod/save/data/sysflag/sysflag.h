#pragma once

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"

#include "logger/logger.h"

template <int32_t size>
struct SysFlagSaveData {

    System::Boolean items[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            items[i] = false;
        }
    }

};

void linkSysFlags(PlayerWork::Object* playerWork);
