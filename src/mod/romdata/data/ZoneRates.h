#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct ZoneRates
    {
        nn::vector<uint32_t> rates;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const ZoneRates& r) {
        j = nn::json {
            {"rates", r.rates},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, ZoneRates& r) {
        j.at("rates").get_to(r.rates);
    }
}
