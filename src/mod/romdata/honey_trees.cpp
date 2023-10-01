#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"
#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"
#include "romdata/data/HoneyTreeEncounters.h"

#include "logger/logger.h"

const char* folderPath = "rom:/Data/ExtraData/Encounters/HoneyTrees/";

void LogHoneyTreeData(const RomData::HoneyTreeZone& z)
{
    Logger::log("CURRENT ZONE\n");
    for (auto &s: z.slots)
    {
        Logger::log("Monsno %d of formno %d (Levels %d-%d)\n", s.monsNo, s.formNo, s.minlv, s.maxlv);
    }
    Logger::log("\n");
}

RomData::HoneyTreeSlot GetHoneyTreeSlot(int32_t zoneID, int32_t slot)
{
    nn::string filePath(folderPath);
    filePath.append("zone_" + nn::to_string(zoneID) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        Logger::log("Parsed Honey Tree data for %s!\n", filePath.c_str());
        RomData::HoneyTreeZone honeyTreeZone = {};
        honeyTreeZone = j.get<RomData::HoneyTreeZone>();
        LogHoneyTreeData(honeyTreeZone);
        Logger::log("Hello0??????\n");

        // Modulo the slot just to be sure
        slot = slot % 10;

        Logger::log("Hello??????\n");
        return honeyTreeZone.slots[slot];
    }
    else
    {
        Logger::log("Error when parsing Honey Tree data!\n");
    }

    // Default
    return {
        .maxlv = 69,
        .minlv = 69,
        .monsNo = array_index(SPECIES, "Ampharos"),
        .formNo = 0,
    };
}