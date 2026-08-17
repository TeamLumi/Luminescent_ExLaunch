#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"

namespace RomData
{
    struct Zone
    {
        int32_t zoneFrame;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const Zone& z) {
        j = nn::json {
            {"zoneFrame", z.zoneFrame},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, Zone& z) {
        j.at("zoneFrame").get_to(z.zoneFrame);
    }
}
