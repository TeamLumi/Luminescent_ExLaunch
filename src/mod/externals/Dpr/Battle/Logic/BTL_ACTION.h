#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"

namespace Dpr::Battle::Logic {
    struct BTL_ACTION : ILClass<BTL_ACTION> {

        struct PARAM : ILStruct<PARAM> {

            struct Fields {
                long raw;
            };

            uint8_t get_gen_cmd() {
                return external<uint8_t>(0x01f4d680, this);
            }
        };

        struct PARAM_OBJ : ILClass<PARAM_OBJ, 0x04c5a7e8> {
            struct Fields {
                PARAM::Object value;
            };
        };
    };
}
