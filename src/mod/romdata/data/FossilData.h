#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct FossilEntry
    {
        int32_t monsNo;
        int32_t formNo;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const FossilEntry& f) {
        j = nn::json {
            {"monsNo", f.monsNo},
            {"formNo", f.formNo},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, FossilEntry& f) {
        j.at("monsNo").get_to(f.monsNo);
        j.at("formNo").get_to(f.formNo);
    }

    struct FossilItemList
    {
        nn::vector<int32_t> items;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const FossilItemList& l) {
        j = nn::json {
            {"items", l.items},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, FossilItemList& l) {
        j.at("items").get_to(l.items);
    }
}
