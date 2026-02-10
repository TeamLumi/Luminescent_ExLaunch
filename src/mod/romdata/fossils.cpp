#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"
#include "romdata/data/FossilData.h"

#include "logger/logger.h"

const char* fossilFilePath = "rom:/Data/ExtraData/Fossils/fossils.json";

static RomData::FossilTable s_fossilTable;
static bool s_loaded = false;

void LoadFossilTable()
{
    if (s_loaded)
        return;

    nn::json j = FsHelper::loadJsonFileFromPath(fossilFilePath);
    if (j != nullptr && !j.is_discarded())
    {
        s_fossilTable = j.get<RomData::FossilTable>();
        s_loaded = true;
        Logger::log("Loaded %d fossil entries\n", (int)s_fossilTable.entries.size());
    }
    else
    {
        Logger::log("Error when parsing Fossil data!\n");
    }
}

const nn::vector<RomData::FossilEntry>& GetFossilEntries()
{
    LoadFossilTable();
    return s_fossilTable.entries;
}

int32_t GetMonsNoFromItemNo(int32_t itemNo)
{
    LoadFossilTable();
    for (const auto& entry : s_fossilTable.entries)
    {
        if (entry.itemNo == itemNo)
            return entry.monsNo;
    }
    return 0;
}
