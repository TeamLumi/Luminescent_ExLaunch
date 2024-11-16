#pragma once
#include "externals/il2cpp-api.h"

namespace DPData {
    struct HONEY_DATA : ILStruct<HONEY_DATA, 0x04c5a498> {
        struct Fields {
            bool Spreaded;
            int32_t Minutes;
            uint8_t TblMonsNo;
            uint8_t RareLv;
            uint8_t SwayLv;
        };
    };
}