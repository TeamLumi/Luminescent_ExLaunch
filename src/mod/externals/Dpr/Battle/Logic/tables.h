#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_FIELD_SITUATION.h"

namespace Dpr::Battle::Logic {
    struct tables : ILClass<tables, game::Address::Dpr_Battle_Logic_tables_TypeInfo> {
        static inline bool IsSakidoriFailWaza(int32_t waza) {
            return external<bool>(game::Address::Dpr_Battle_Logic_tables_IsSakidoriFailWaza, waza);
        }
    };
}