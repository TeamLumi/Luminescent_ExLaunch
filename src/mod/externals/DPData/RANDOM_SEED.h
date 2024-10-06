#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace DPData {
    struct RANDOM_SEED : ILStruct<RANDOM_SEED> {
        struct Fields {
            System::String::Object* group_name;
            System::String::Object* name;
            int32_t sex;
            int32_t region_code;
            uint64_t seed;
            uint64_t random;
            int64_t time_stmp;
            int32_t user_id;
        };
        static_assert(offsetof(Fields, user_id) == 0x30);
    };
}
