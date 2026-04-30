#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"
#include "romdata/data/EncounterTableData.h"
#include "externals/UnityEngine/Random.h"

#include "logger/logger.h"

const char* encounterTablesFolderPath = "rom:/Data/ExtraData/Encounters/Tables/";

int32_t GetEncounterTableCount(int32_t tableID)
{
    nn::string metaPath(encounterTablesFolderPath);
    metaPath.append(nn::to_string(tableID) + "/meta.json");

    nn::json metaJson = FsHelper::loadJsonFileFromPath(metaPath.c_str());
    if (metaJson != nullptr && !metaJson.is_discarded())
    {
        RomData::EncounterTableMeta meta = {};
        meta = metaJson.get<RomData::EncounterTableMeta>();
        return meta.slotCount;
    }

    return 0;
}

RomData::EncounterSlot GetEncounterTableSlot(int32_t tableID)
{
    int32_t slotCount = GetEncounterTableCount(tableID);

    if (slotCount > 0)
    {
        // Pick a random slot index
        int32_t slotIndex = UnityEngine::Random::Range(0, slotCount);

        // Load the individual slot file
        nn::string slotPath(encounterTablesFolderPath);
        slotPath.append(nn::to_string(tableID) + "/" + nn::to_string(slotIndex) + ".json");

        nn::json slotJson = FsHelper::loadJsonFileFromPath(slotPath.c_str());
        if (slotJson != nullptr && !slotJson.is_discarded())
        {
            RomData::EncounterSlot slot = {};
            slot = slotJson.get<RomData::EncounterSlot>();

            Logger::log("GetEncounterTableSlot: Table %d, Slot %d/%d, MonsNo %d, FormNo %d\n",
                tableID, slotIndex, slotCount, slot.monsNo, slot.formNo);
            return slot;
        }
        else
        {
            Logger::log("GetEncounterTableSlot: Error parsing slot %d for table %d!\n", slotIndex, tableID);
        }
    }
    else
    {
        Logger::log("GetEncounterTableSlot: Table %d has no slots!\n", tableID);
    }

    // Default - return Bulbasaur
    return {
        .monsNo = 1,
        .formNo = 0,
    };
}
