#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/Primitives.h"
#include "nn/account.h"

namespace Dpr::NX {
    struct UserAccount : ILClass<UserAccount, 0x04c600c0> {

        struct StaticFields {
            nn::account::Uid2::Object UserId;
            nn::account::UserHandle::Object UserHandle;
            nn::account::Nickname::Object Nickname;
        };

        static_assert(offsetof(StaticFields, Nickname) == 0x28);

        static inline void Open() {
            external<void>(0x01de02b0);
        }
    };
}
