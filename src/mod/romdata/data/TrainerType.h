#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct TrainerType
    {
        int32_t reserved;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const TrainerType& t) {
        j = nn::json {
            {"reserved", t.reserved},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, TrainerType& t) {
        j.at("reserved").get_to(t.reserved);
    }
}
