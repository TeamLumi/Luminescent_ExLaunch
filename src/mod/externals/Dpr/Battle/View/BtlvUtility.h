#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::View {
    struct BtlvUtility : ILClass<BtlvUtility> {
        struct Fields {
        };

        inline int32_t GetUniqueWazaDataTurnType(int32_t wazaNo,int32_t monsNo,uint8_t formNo,int32_t turnType) {
            return external<int32_t>(game::Address::Dpr_Battle_View_BtlvUtility_GetUniqueWazaDataTurnType, this, wazaNo, monsNo, formNo, turnType);
        }
    };
}