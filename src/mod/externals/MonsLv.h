#pragma once

#include "externals/il2cpp-api.h"

struct MonsLv : ILStruct<MonsLv, game::Address::MonsLv_Array_TypeInfo> {
    struct Fields {
        int32_t maxlv;
        int32_t minlv;
        int32_t monsNo;
    };
};
