#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/Zone.h"

#include "logger/logger.h"

const char* zoneFolderPath = "rom:/Data/ExtraData/ZoneData/";

void LogZoneData(const RomData::Zone& z)
{
    Logger::log("CURRENT ZONE\n");
    Logger::log("Zone Window Frame: %d\n", z.zoneFrame);
}

RomData::Zone GetZoneData(int32_t zoneID)
{
    nn::string filePath(zoneFolderPath);
    filePath.append("zone_" + nn::to_string(zoneID) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::Zone zoneData = {};
        zoneData = j.get<RomData::Zone>();

        return zoneData;
    }
    else
    {
        Logger::log("Error when parsing Zone data!\n");
    }

    // Default: Base Window Frame
    return {
        .zoneFrame = 0,
    };
}