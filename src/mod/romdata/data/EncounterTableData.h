#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"

namespace RomData
{
    struct EncounterSlot
    {
        int32_t monsNo;
        int32_t formNo;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const EncounterSlot& s) {
        j = nn::json {
            {"monsNo", s.monsNo},
            {"formNo", s.formNo},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, EncounterSlot& s) {
        j.at("monsNo").get_to(s.monsNo);
        j.at("formNo").get_to(s.formNo);
    }

    struct EncounterTableMeta
    {
        int32_t slotCount;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const EncounterTableMeta& m) {
        j = nn::json {
            {"slotCount", m.slotCount},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, EncounterTableMeta& m) {
        j.at("slotCount").get_to(m.slotCount);
    }
}
