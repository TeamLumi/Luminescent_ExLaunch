#pragma once
#include "externals/il2cpp-api.h"

namespace DPData {
    struct HONEY_DATA : ILStruct<HONEY_DATA> {
        struct Fields {
            bool Spreaded;
            int32_t Minutes;
            uint8_t TblMonsNo;
            uint8_t RareLv;
            uint8_t SwayLv;
        };
    };
}