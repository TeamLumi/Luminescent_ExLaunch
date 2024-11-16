#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData {
    struct RoamerData {
        int32_t monsNo;
        int32_t formNo;
        int32_t lvl;
        nn::vector<int32_t> moves;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const RoamerData& s) {
        j = nn::json {
            {"monsNo", s.monsNo},
            {"formNo", s.formNo},
            {"lvl", s.lvl},
            {"moves", s.moves}
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, RoamerData& s) {
        j.at("monsNo").get_to(s.monsNo);
        j.at("formNo").get_to(s.formNo);
        j.at("lvl").get_to(s.lvl);
        j.at("moves").get_to(s.moves);
    }
}
