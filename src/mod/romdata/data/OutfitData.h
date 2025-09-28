#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct OutfitData {
        bool forMale;
        bool forFemale;
        bool isLockedByFlag;
        int32_t flag;
    };

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, OutfitData& o) {
        j.at("forMale").get_to(o.forMale);
        j.at("forFemale").get_to(o.forFemale);
        j.at("isLockedByFlag").get_to(o.isLockedByFlag);
        j.at("flag").get_to(o.flag);
    }


}
