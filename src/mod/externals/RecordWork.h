#pragma once

#include "externals/il2cpp-api.h"

struct RecordWork : ILClass<RecordWork> {

    static inline uint32_t Get(int32_t index) {
        return external<uint32_t>(0x019a4660, index);
    }

};
