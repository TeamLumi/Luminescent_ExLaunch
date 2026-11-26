#pragma once

#include "externals/il2cpp-api.h"

struct AttributeID : ILClass<AttributeID> {
    static inline bool MATR_IsWater(int32_t matr_code) {
        return external<bool>(game::Address::AttributeID_MATR_IsWater, matr_code);
    }

    static inline bool MATR_IsGrass(int32_t matr_code) {
        return external<bool>(game::Address::AttributeID_MATR_IsGrass, matr_code);
    }

    static inline bool MATR_IsDunFloor(int32_t matr_code) {
        return external<bool>(game::Address::AttributeID_MATR_IsDunFloor, matr_code);
    }
};
