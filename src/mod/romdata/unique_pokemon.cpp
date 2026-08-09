#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/UniquePokemon.h"

#include "logger/logger.h"

const char* uniquePokemonFolderPath = "rom:/Data/ExtraData/MonData/UniquePokemon/";

void LogUniquePokemonData(const RomData::UniquePokemon& t)
{
    Logger::log("CURRENT UNIQUE POKEMON\n");
}

RomData::UniquePokemon GetUniquePokemon(int32_t index)
{
    nn::string filePath(uniquePokemonFolderPath);
    filePath.append(nn::to_string(index) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::UniquePokemon uniquePokemonData = {};
        uniquePokemonData = j.get<RomData::UniquePokemon>();
        LogUniquePokemonData(uniquePokemonData);

        return uniquePokemonData;
    }
    else
    {
        Logger::log("Error when parsing UniquePokemon data!\n");
    }
    // Defaults
    return {
        .monsNo = 1,
        .formNo = 0,
        .level = 5,
        .gender = 255,
        .nature = 65535,
        .ability = -1,
        .maxIvs = 0,
        .ivs = {},
        .trainerId = -1,
        .trainerLabel = "",
        .nicknameLabel = "",
        .formArg = 0,
        .item = 0,
        .langId = 0,
        .ballId = 0,
        .evs = {},
        .contestStats = {},
        .moves = {},
        .ribbons = {},
        .shiny = -1,
        .placeNo = -1,
        .friendship = 0,
        .pokerus = 0,
        .fateful = false,
    };
}