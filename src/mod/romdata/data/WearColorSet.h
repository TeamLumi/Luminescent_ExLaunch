#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"
#include "romdata/data/Color.h"

namespace RomData
{
    struct WearColorSet
    {
        Color color0;
        Color color1;
        Color color2;
        Color color3;
        Color color4;
        Color color5;
        Color color6;
        Color color7;
        Color color8;
        Color color9;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const WearColorSet& s) {
        j = nn::json {
            {"color0", s.color0},
            {"color1", s.color1},
            {"color2", s.color2},
            {"color3", s.color3},
            {"color4", s.color4},
            {"color5", s.color5},
            {"color6", s.color6},
            {"color7", s.color7},
            {"color8", s.color8},
            {"color9", s.color9},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, WearColorSet& s) {
        j.at("color0").get_to(s.color0);
        j.at("color1").get_to(s.color1);
        j.at("color2").get_to(s.color2);
        j.at("color3").get_to(s.color3);
        j.at("color4").get_to(s.color4);
        j.at("color5").get_to(s.color5);
        j.at("color6").get_to(s.color6);
        j.at("color7").get_to(s.color7);
        j.at("color8").get_to(s.color8);
        j.at("color9").get_to(s.color9);
    }
}
