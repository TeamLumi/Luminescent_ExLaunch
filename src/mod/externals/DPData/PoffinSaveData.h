#pragma once

#include "externals/il2cpp-api.h"
#include "externals/DPData/PoffinData.h"

namespace DPData {
    struct PoffinSaveData : ILStruct<PoffinSaveData> {
        struct Fields {
            DPData::PoffinData::Array* Poffins;
            int32_t CookingCount;
        };

        inline bool AddPoffin(DPData::PoffinData::Object poffin) {
            return external<bool>(0x0229b1b0, this, poffin);
        }

        inline DPData::PoffinData::Object GetPoffin(int32_t index) {
            return external<DPData::PoffinData::Object>(0x0229bb80, this, index);
        }
    };
}
