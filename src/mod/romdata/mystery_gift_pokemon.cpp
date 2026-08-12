#include "exlaunch.hpp"

#include "helpers.h"

#include "logger/logger.h"

#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/MysteryGiftPokemon.h"

const char* mysteryGiftPokemonFolderPath = "rom:/Data/ExtraData/MonData/MysteryGiftPokemon/";

void LogMysteryGiftPokemonData(const RomData::MysteryGiftPokemon& t)
{
    Logger::log("CURRENT MYSTERY GIFT POKEMON\n");
}

RomData::MysteryGiftPokemon GetMysteryGiftPokemon(int32_t index)
{
    nn::string filePath(mysteryGiftPokemonFolderPath);
    filePath.append(nn::to_string(index) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::MysteryGiftPokemon mysteryGiftPokemonData = {};
        mysteryGiftPokemonData = j.get<RomData::MysteryGiftPokemon>();
        LogMysteryGiftPokemonData(mysteryGiftPokemonData);

        return mysteryGiftPokemonData;
    }
    else
    {
        Logger::log("Error when parsing MysteryGiftPokemon data!\n");
    }
    // Defaults
    return {
        .monsNo = 1,
        .formNo = 0,
        .level = 1,
        .originalLevel = 1,
        .isEgg = false,
        .wasEgg = false,
        .gender = 255,
        .nature = 65535,
        .ability = -1,
        .rand = 0,
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
        .metYear = 1,
        .metMonth = 1,
        .metDay = 1,
        .eggYear = 1,
        .eggMonth = 1,
        .eggDay = 1,
        .eggPlaceNo = 65535,
    };
}