#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BgComponentData.h"

namespace Dpr::Battle::Logic {
    struct BTL_FIELD_SITUATION : ILClass<BTL_FIELD_SITUATION> {
        struct Fields {
            BgComponentData::Object* bgComponent;
            uint8_t weather;
            uint8_t fieldWeather;
            uint8_t ground;
        };
    };
}
