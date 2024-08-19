#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/FormHeldItemMon.h"
#include "romdata/data/UnbreakablePokeItem.h"

#include "logger/logger.h"

const char* formHeldItemFolderPath = "rom:/Data/ExtraData/MonData/FormHeldItem/";
const char* unbreakablePokeItemFolderPath = "rom:/Data/ExtraData/MonData/UnbreakablePokeItem/";

void LogFormHeldItemMonData(const RomData::FormHeldItemMon& m)
{
    Logger::log("CURRENT MON\n");
    Logger::log("Default form: %d\n", m.defaultForm);
    for (auto &i: m.itemForms)
    {
        Logger::log("Item %d for Form %d\n", i.itemNo, i.formNo);
    }
    Logger::log("\n");
}

void LogUnbreakablePokeItem(const RomData::UnbreakablePokeItem& u)
{
    Logger::log("CURRENT MON\n");
    for (auto &i: u.items)
    {
        Logger::log("Item %d\n", i);
    }
    Logger::log("\n");
}

RomData::FormHeldItemMon GetFormHeldItemMon(int32_t monsNo)
{
    nn::string filePath(formHeldItemFolderPath);
    filePath.append("monsno_" + nn::to_string(monsNo) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::FormHeldItemMon formHeldItemMon = {};
        formHeldItemMon = j.get<RomData::FormHeldItemMon>();

        return formHeldItemMon;
    }

    // Default
    return {
        .defaultForm = 0,
        .itemForms = {},
    };
}

RomData::UnbreakablePokeItem GetUnbreakablePokeItems(int32_t monsNo)
{
    nn::string filePath(unbreakablePokeItemFolderPath);
    filePath.append("monsno_" + nn::to_string(monsNo) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::UnbreakablePokeItem unbreakablePokeItem = {};
        unbreakablePokeItem = j.get<RomData::UnbreakablePokeItem>();

        return unbreakablePokeItem;
    }

    // Default
    return {
        .items = {},
    };
}