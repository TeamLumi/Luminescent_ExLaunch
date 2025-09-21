#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct Trainer
    {
        int32_t wearVariation;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const Trainer& t) {
        j = nn::json {
            {"wearVariation", t.wearVariation},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, Trainer& t) {
        j.at("wearVariation").get_to(t.wearVariation);
    }
}
