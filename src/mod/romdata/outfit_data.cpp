#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/OutfitData.h"

#include "logger/logger.h"

const char* outfitDataFolderPath = "rom:/Data/ExtraData/Outfits/";

RomData::OutfitData GetOutfitData(int32_t dressID) {
    nn::string filePath(outfitDataFolderPath);
    filePath.append("outfit_" + nn::to_string(dressID) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded()) {
        RomData::OutfitData outfitData{};
        outfitData = j.get<RomData::OutfitData>();
        return outfitData;

    }

    else {
        Logger::log("[JSON Outfits] %s was not found. Excluding entry from selection.\n", filePath.c_str());
        return {
                .forMale = false,
                .forFemale = false,
                .isLockedByFlag = false,
                .flag = 0
        };
    }
}