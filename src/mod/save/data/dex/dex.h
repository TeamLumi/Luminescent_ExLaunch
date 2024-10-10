#pragma once

#include "externals/DPData/GET_STATUS.h"
#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"

#include "logger/logger.h"

struct DexSaveDataElement {
    DPData::GET_STATUS get_status;
    System::Boolean male_color_flag;
    System::Boolean female_color_flag;
    System::Boolean male_flag;
    System::Boolean female_flag;
    System::Int32 language_flags;

    void Initialize() {
        get_status = 0;
        male_color_flag = false;
        female_color_flag = false;
        male_flag = false;
        female_flag = false;
        language_flags = 0;
    }
};

template <int32_t size>
struct DexSaveData {

    DexSaveDataElement elements[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            elements[i].Initialize();
        }
    }
};

void linkZukan(PlayerWork::Object* playerWork);
