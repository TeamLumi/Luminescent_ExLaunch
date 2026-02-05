#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/FormHeldItemMon.h"
#include "romdata/data/UnbreakablePokeItem.h"

#include "logger/logger.h"

const char* formHeldItemFolderPath = "rom:/Data/ExtraData/MonData/FormHeldItem/";
const char* unbreakablePokeItemFolderPath = "rom:/Data/ExtraData/MonData/UnbreakablePokeItem/";
const char* alwaysUnbreakablePath = "rom:/Data/ExtraData/MonData/UnbreakablePokeItem/always.json";

static RomData::UnbreakablePokeItem s_alwaysUnbreakable = { .items = {} };
static bool s_alwaysLoaded = false;

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
    else
    {
        Logger::log("Error when parsing Form Held Item data!\n");
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
    else
    {
        Logger::log("Error when parsing Form Held Item data!\n");
    }

    // Default
    return {
        .items = {},
    };
}

RomData::UnbreakablePokeItem GetAlwaysUnbreakableItems()
{
    if (!s_alwaysLoaded)
    {
        s_alwaysLoaded = true;
        nn::json j = FsHelper::loadJsonFileFromPath(alwaysUnbreakablePath);
        if (j != nullptr && !j.is_discarded())
        {
            s_alwaysUnbreakable = j.get<RomData::UnbreakablePokeItem>();
        }
    }

    return s_alwaysUnbreakable;
}