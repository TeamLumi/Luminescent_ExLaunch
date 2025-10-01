#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct MoveTutor
    {
        nn::vector<int32_t> moves;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const MoveTutor& t) {
        j = nn::json {
            {"moves", t.moves},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, MoveTutor& t) {
        j.at("moves").get_to(t.moves);
    }
}
