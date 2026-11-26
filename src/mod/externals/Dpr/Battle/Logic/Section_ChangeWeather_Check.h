#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/ChangeWeatherResult.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_ChangeWeather_Check : ILClass<Section_ChangeWeather_Check, game::Address::Dpr_Battle_Logic_Section_ChangeWeather_Check_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_ChangeWeather_Check_Description_TypeInfo> {
            struct Fields {
                uint8_t weather;
                uint8_t turn;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_ChangeWeather_Check_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_ChangeWeather_Check_Result_TypeInfo> {
            struct Fields {
                ChangeWeatherResult result;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_ChangeWeather_Check_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
