#pragma once
#include "externals/il2cpp-api.h"
#include "types.h"
#include "externals/System/String.h"

namespace nn::account {
    struct UserHandle : ILStruct<UserHandle> {
        struct Fields {
            uint64_t _data0;
            uint64_t _data1;
            uint64_t _context;
        };
    };

    struct Uid2 : ILStruct<Uid2> {
        struct Fields {
            uint64_t _data0;
            uint64_t _data1;
        };
    };

    struct Nickname : ILStruct<Nickname> {
        struct Fields {
            System::String::Object* name;
        };
    };

    /// Account UserId.
    typedef struct {
        u64 uid[2];       ///< UserId. All-zero is invalid / Uid not set. See also \ref accountUidIsValid.
    } Uid;

}