#pragma once

#include "externals/il2cpp-api.h"

struct GFL : ILClass<GFL> {
    struct Fields {};

    static inline void ASSERT(bool condition) {
        external<void>(game::Address::GFL_ASSERT, condition);
    }
};
