#pragma once

#include "externals/il2cpp-api.h"

struct BadgeWork : ILClass<BadgeWork> {
    static inline bool IsGet(int32_t id) {
        return external<bool>(game::Address::BadgeWork_IsGet, id);
    }
};