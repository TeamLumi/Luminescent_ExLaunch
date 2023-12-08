#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION_PARAM.h"

namespace Dpr::Battle::Logic {
    struct BTL_ACTION_PARAM_OBJ : ILClass<BTL_ACTION_PARAM_OBJ> {
        struct Fields {
            BTL_ACTION_PARAM::Object value;
        };
    };
}
