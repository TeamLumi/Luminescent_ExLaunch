#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"

namespace RomData
{
    struct BoxOpenParam
    {
        int32_t tradeId;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const BoxOpenParam& p) {
        j = nn::json {
            {"tradeId", p.tradeId},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, BoxOpenParam& p) {
        j.at("tradeId").get_to(p.tradeId);
    }
}
