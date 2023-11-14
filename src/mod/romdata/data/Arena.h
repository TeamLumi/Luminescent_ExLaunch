#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"

namespace RomData
{
    struct Arena
    {
        int32_t secretPowerMove;
        uint8_t camouflageType;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const Arena& i) {
        j = nn::json {
            {"secretPowerMove", i.secretPowerMove},
            {"camouflageType", i.camouflageType},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, Arena& i) {
        j.at("secretPowerMove").get_to(i.secretPowerMove);
        j.at("camouflageType").get_to(i.camouflageType);
    }
}
