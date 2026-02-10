#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct FossilEntry
    {
        int32_t itemNo;
        int32_t monsNo;
        int32_t formNo;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const FossilEntry& f) {
        j = nn::json {
            {"itemNo", f.itemNo},
            {"monsNo", f.monsNo},
            {"formNo", f.formNo},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, FossilEntry& f) {
        j.at("itemNo").get_to(f.itemNo);
        j.at("monsNo").get_to(f.monsNo);
        j.at("formNo").get_to(f.formNo);
    }

    struct FossilTable
    {
        nn::vector<FossilEntry> entries;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const FossilTable& t) {
        j = nn::json {
            {"entries", t.entries},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, FossilTable& t) {
        j.at("entries").get_to(t.entries);
    }
}
