#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct SmeargleColor
    {
        int32_t baseColor;
        float hueShiftNegative;
        float hueShiftPositive;
        float saturationShiftNegative;
        float saturationShiftPositive;
        float valueShiftNegative;
        float valueShiftPositive;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const SmeargleColor& c) {
        j = nn::json {
            {"baseColor", c.baseColor},
            {"hueShiftNegative", c.hueShiftNegative},
            {"hueShiftPositive", c.hueShiftPositive},
            {"saturationShiftNegative", c.saturationShiftNegative},
            {"saturationShiftPositive", c.saturationShiftPositive},
            {"valueShiftNegative", c.valueShiftNegative},
            {"valueShiftPositive", c.valueShiftPositive},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, SmeargleColor& c) {
        j.at("baseColor").get_to(c.baseColor);
        j.at("hueShiftNegative").get_to(c.hueShiftNegative);
        j.at("hueShiftPositive").get_to(c.hueShiftPositive);
        j.at("saturationShiftNegative").get_to(c.saturationShiftNegative);
        j.at("saturationShiftPositive").get_to(c.saturationShiftPositive);
        j.at("valueShiftNegative").get_to(c.valueShiftNegative);
        j.at("valueShiftPositive").get_to(c.valueShiftPositive);
    }
}
