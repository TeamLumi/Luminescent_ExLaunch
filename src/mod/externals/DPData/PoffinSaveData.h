#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/PoffinData.h"

namespace DPData {
    struct PoffinSaveData : ILStruct<PoffinSaveData> {
        struct DisplayClass5_0 : ILClass<DisplayClass5_0, game::Address::DPData_PoffinSaveData_c_DisplayClass5_0_TypeInfo> {
            struct Fields {
                PoffinData::Object poffin;
            };

            static inline StaticILMethod<game::Address::Method_DPData_PoffinSaveData_c_DisplayClass5_0_DelPoffin_b_0, PoffinData> Method$$DelPoffin_b__0 {};

            inline void ctor() {
                external<void>(game::Address::DPData_PoffinSaveData_c_DisplayClass5_0_ctor, this);
            }
        };

        struct Fields {
            DPData::PoffinData::Array* Poffins;
            int32_t CookingCount;
        };

        inline bool AddPoffin(DPData::PoffinData::Object poffin) {
            return external<bool>(game::Address::DPData_PoffinSaveData_AddPoffin, this, poffin);
        }

        inline DPData::PoffinData::Object GetPoffin(int32_t index) {
            return external<DPData::PoffinData::Object>(game::Address::DPData_PoffinSaveData_GetPoffin, this, index);
        }

        inline DPData::PoffinData::Array* GetHavePoffins() {
            return external<DPData::PoffinData::Array*>(game::Address::DPData_PoffinSaveData_GetHavePoffins, this);
        }

        inline bool DelPoffin(DPData::PoffinData::Object poffin) {
            return external<bool>(game::Address::DPData_PoffinSaveData_DelPoffin, this, poffin);
        }
    };
}
