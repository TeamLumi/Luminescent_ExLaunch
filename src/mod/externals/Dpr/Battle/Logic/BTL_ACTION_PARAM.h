#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct BTL_ACTION_PARAM : ILStruct<BTL_ACTION_PARAM, 0x04c5a7e8> {
        struct Fields {
            long raw;
        };

        uint8_t get_gen_cmd() {
            return external<uint8_t>(0x01f4d680, this);
        }

        void set_fight_cmd(uint8_t value) {
            external<void>(0x01f4d580, this, value);
        }
    };
}
