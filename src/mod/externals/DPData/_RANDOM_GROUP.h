#pragma once
#include "externals/DPData/RANDOM_SEED.h"

namespace DPData {
    struct _RANDOM_GROUP : ILStruct<_RANDOM_GROUP> {
        struct Fields {
            DPData::RANDOM_SEED::Array* group;
        };

        static long GetByteCount() {
            return RANDOM_SEED::GetByteCount() * 12;
        }
    };
}
