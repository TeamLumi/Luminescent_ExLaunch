#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/PoffinData.h"

namespace DPData {
    struct PoffinSaveData : ILStruct<PoffinSaveData> {
        struct DisplayClass5_0 : ILClass<DisplayClass5_0, 0x04c5a538> {
            struct Fields {
                PoffinData::Object poffin;
            };

            static inline StaticILMethod<0x04c6ac60, PoffinData> Method$$DelPoffin_b__0 {};

            inline void ctor() {
                external<void>(0x0229b660, this);
            }
        };

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

        inline DPData::PoffinData::Array* GetHavePoffins() {
            return external<DPData::PoffinData::Array*>(0x0229b430, this);
        }
    };
}
