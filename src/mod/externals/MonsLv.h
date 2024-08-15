#pragma once

#include "externals/il2cpp-api.h"

struct MonsLv : ILStruct<MonsLv, 0x04c5f208> {
    struct Fields {
        int32_t maxlv;
        int32_t minlv;
        int32_t monsNo;
    };
};
