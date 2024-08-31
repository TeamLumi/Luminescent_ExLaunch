#pragma once
#include "externals/il2cpp-api.h"
#include "BSP_TRAINER_DATA_CORE_DATA.h"


namespace Dpr::Battle::Logic {
    struct BSP_TRAINER_DATA : ILClass<BSP_TRAINER_DATA> {
        struct Fields {
            BSP_TRAINER_DATA_CORE_DATA::Object* mCore;
        };
    };

}
