#pragma once
#include "externals/il2cpp-api.h"
#include "BSP_TRAINER_DATA_CORE_DATA.h"


namespace Dpr::Battle::Logic {
    struct BSP_TRAINER_DATA : ILClass<BSP_TRAINER_DATA> {
        struct Fields {
            BSP_TRAINER_DATA_CORE_DATA::Object* mCore;
        };

        inline void SetColorID(int32_t color_id) {
            external<void>(0x01ac46c0, this, color_id);
        }

        inline int32_t GetColorID() {
            return external<int32_t>(0x01ac4610, this);
        }
    };

}
