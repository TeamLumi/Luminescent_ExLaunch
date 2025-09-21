#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"
#include "romdata/data/WearColorSet.h"

namespace RomData
{
    struct PlayerWearColorSet
    {
        WearColorSet wearField;
        WearColorSet wearBattle;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const PlayerWearColorSet& s) {
        j = nn::json {
            {"wearField", s.wearField},
            {"wearBattle", s.wearBattle},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, PlayerWearColorSet& s) {
        j.at("wearField").get_to(s.wearField);
        j.at("wearBattle").get_to(s.wearBattle);
    }
}
