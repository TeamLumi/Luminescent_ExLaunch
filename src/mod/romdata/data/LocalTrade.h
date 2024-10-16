#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct LocalTrade
    {
        nn::vector<uint16_t> ivs;
        nn::vector<uint32_t> evs;
        nn::vector<uint8_t> contestStats;
        uint32_t ballId;
        bool forceShiny;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const LocalTrade& t) {
        j = nn::json {
            {"ivs", t.ivs},
            {"evs", t.evs},
            {"contestStats", t.contestStats},
            {"ballId", t.ballId},
            {"forceShiny", t.forceShiny},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, LocalTrade& t) {
        j.at("ivs").get_to(t.ivs);
        j.at("evs").get_to(t.evs);
        j.at("contestStats").get_to(t.contestStats);
        j.at("ballId").get_to(t.ballId);
        j.at("forceShiny").get_to(t.forceShiny);
    }
}
