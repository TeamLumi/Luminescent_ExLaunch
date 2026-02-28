#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"
#include "romdata/data/FossilData.h"

#include "logger/logger.h"

const char* fossilFolderPath = "rom:/Data/ExtraData/Fossils/";

RomData::FossilEntry GetFossilEntry(int32_t itemNo)
{
    nn::string filePath(fossilFolderPath);
    filePath.append(nn::to_string(itemNo) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::FossilEntry entry = {};
        entry = j.get<RomData::FossilEntry>();
        return entry;
    }
    else
    {
        Logger::log("Error when parsing Fossil data for item %d!\n", itemNo);
    }

    return {
        .monsNo = 0,
        .formNo = 0,
    };
}

int32_t GetMonsNoFromItemNo(int32_t itemNo)
{
    RomData::FossilEntry entry = GetFossilEntry(itemNo);
    return entry.monsNo;
}

int32_t GetFossilFormNoFromItemNo(int32_t itemNo)
{
    RomData::FossilEntry entry = GetFossilEntry(itemNo);
    return entry.formNo;
}

RomData::FossilItemList LoadFossilItemList()
{
    nn::string filePath(fossilFolderPath);
    filePath.append("fossil_items.json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::FossilItemList list = {};
        list = j.get<RomData::FossilItemList>();
        Logger::log("Loaded %d fossil item numbers\n", (int)list.items.size());
        return list;
    }
    else
    {
        Logger::log("Error when parsing fossil item list!\n");
    }

    return {};
}
