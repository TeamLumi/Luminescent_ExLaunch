#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/Arena.h"

#include "logger/logger.h"

const char* arenaFolderPath = "rom:/Data/ExtraData/ArenaData/";

void LogArenaData(const RomData::Arena& a)
{
    Logger::log("CURRENT ARENA\n");
    Logger::log("Secret Power Move: %d\nCamouflage Type: %d\n", a.secretPowerMove, a.camouflageType);
}

RomData::Arena GetExtraArenaData(int32_t arena)
{
    nn::string filePath(arenaFolderPath);
    filePath.append("arena_" + nn::to_string(arena) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::Arena arenaData = {};
        arenaData = j.get<RomData::Arena>();

        return arenaData;
    }
    else
    {
        Logger::log("Error when parsing Arena data!\n");
    }

    // Default: Spit Up, Normal
    return {
        .secretPowerMove = 255,
        .camouflageType = 0,
    };
}