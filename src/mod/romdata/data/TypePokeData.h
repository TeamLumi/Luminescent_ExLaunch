#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct TypePokeSlot
    {
        int32_t monsNo;
        int32_t formNo;
    };

    struct TypePokeList
    {
        nn::vector<TypePokeSlot> slots;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const TypePokeSlot& s) {
        j = nn::json {
            {"monsNo", s.monsNo},
            {"formNo", s.formNo},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, TypePokeSlot& s) {
        j.at("monsNo").get_to(s.monsNo);
        j.at("formNo").get_to(s.formNo);
    }

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const TypePokeList& t) {
        j = nn::json {
            {"slots", t.slots},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, TypePokeList& t) {
        j.at("slots").get_to(t.slots);
    }
}
