#pragma once

namespace RomData
{
    struct Color
    {
        float r;
        float g;
        float b;
        float a;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const Color& c) {
        j = nn::json {
            {"r", c.r},
            {"g", c.g},
            {"b", c.b},
            {"a", c.a},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, Color& c) {
        j.at("r").get_to(c.r);
        j.at("g").get_to(c.g);
        j.at("b").get_to(c.b);
        j.at("a").get_to(c.a);
    }
}