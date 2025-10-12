#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/MV_POKE_DATA.h"
#include "externals/Pml/PokePara/PokemonParam.h"

struct EncountDataWork : ILClass<EncountDataWork> {
    static inline int32_t GetUrayamaIndex(int32_t index) {
        return external<int32_t>(0x01f09de0, index);
    }
    
    static inline bool EncDataSave_CanUseSpray() {
        return external<bool>(0x01f08ac0);
    }
    
    static inline bool GetMovePokeData(int32_t index, DPData::MV_POKE_DATA::Object *outData) {
        return external<bool>(0x01f08e10, index, outData);
    }
    
    static inline bool IsTairyouHassei() {
        return external<bool>(0x01f09d60);
    }

    static inline void SetMovePokeData_ZoneID(int32_t index, int32_t zonid) {
        external<void>(0x01f08f00, index, zonid);
    }

    static inline bool IsMovePokeData_ReActive(int32_t index) {
        return external<bool>(0x01f091c0, index);
    }

    static inline void SetMovePokeData_Status(int32_t index, int32_t status) {
        external<void>(0x01f09110, index, status);
    }

    static inline void SetMovePokeData(Pml::PokePara::PokemonParam::Object* param, bool init) {
        external<void>(0x01f08b60, param, init);
    }

    static inline void MovePoke_RandomZone(int32_t monsno) {
        external<void>(0x01f09270, monsno);
    }

    static inline int32_t GetMovePoke_ZoneID(int32_t index) {
        return external<int32_t>(0x01f099b0, index);
    }
};
