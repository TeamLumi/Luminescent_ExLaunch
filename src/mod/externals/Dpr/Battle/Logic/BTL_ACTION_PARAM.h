#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct BTL_ACTION_PARAM : ILStruct<BTL_ACTION_PARAM, game::Address::Dpr_Battle_Logic_BTL_ACTION_PARAM_OBJ_TypeInfo> {
        struct Fields {
            long raw;
        };

        uint8_t get_gen_cmd() {
            return external<uint8_t>(game::Address::Dpr_Battle_Logic_BTL_ACTION_PARAM_get_gen_cmd, this);
        }

        void set_fight_cmd(uint8_t value) {
            external<void>(game::Address::Dpr_Battle_Logic_BTL_ACTION_PARAM_set_fight_cmd, this, value);
        }
    };
}
