#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/MV_POKE_DATA.h"

struct EncountDataWork : ILClass<EncountDataWork> {
    static inline int32_t GetUrayamaIndex(int32_t index) {
        return external<int32_t>(game::Address::EncountDataWork_GetUrayamaIndex, index);
    }
    
    static inline bool EncDataSave_CanUseSpray() {
        return external<bool>(game::Address::EncountDataWork_EncDataSave_CanUseSpray);
    }
    
    static inline bool GetMovePokeData(int32_t index, DPData::MV_POKE_DATA::Object *outData) {
        return external<bool>(game::Address::EncountDataWork_GetMovePokeData, index, outData);
    }
    
    static inline bool IsTairyouHassei() {
        return external<bool>(game::Address::EncountDataWork_IsTairyouHassei);
    }
};
