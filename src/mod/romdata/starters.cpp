#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/Starter.h"

#include "logger/logger.h"

const char* starterFolderPath = "rom:/Data/ExtraData/Encounters/Starter/";

RomData::Starter GetStarter(int32_t index)
{
    nn::string filePath(starterFolderPath);
    filePath.append("starter_" + nn::to_string(index) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::Starter starter = {};
        starter = j.get<RomData::Starter>();

        return starter;
    }
    else
    {
        Logger::log("Error when parsing Starter data!\n");
    }

    // Default - Oddish
    return {
        .monsNo = 43,
        .formNo = 0,
        .level = 5,
        .maxIVs = 3,
        .itemNo = 0,
    };
}