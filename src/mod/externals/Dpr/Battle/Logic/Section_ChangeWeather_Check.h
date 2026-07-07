#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/ChangeWeatherResult.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_ChangeWeather_Check : ILClass<Section_ChangeWeather_Check, 0x04c5b6a0> {
        struct Description : ILClass<Description, 0x04c5be10> {
            struct Fields {
                uint8_t weather;
                uint8_t turn;
            };

            inline void ctor() {
                external<void>(0x021ae690, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5be18> {
            struct Fields {
                ChangeWeatherResult result;
            };

            inline void ctor() {
                external<void>(0x021ae6c0, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
