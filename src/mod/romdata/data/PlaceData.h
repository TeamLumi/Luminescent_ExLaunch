#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct PlaceData
    {
        int32_t wearVariation;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const PlaceData& p) {
        j = nn::json {
            {"wearVariation", p.wearVariation},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, PlaceData& p) {
        j.at("wearVariation").get_to(p.wearVariation);
    }
}
