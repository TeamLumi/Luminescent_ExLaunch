#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/ColorSet.h"

#include "logger/logger.h"

const char* colorVariationFolderPath = "rom:/Data/ExtraData/ColorVariation/";

RomData::ColorSet GetColorSet(int32_t index)
{
    nn::string filePath(colorVariationFolderPath);
    filePath.append("set_" + nn::to_string(index) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::ColorSet colorSet = {};
        colorSet = j.get<RomData::ColorSet>();

        return colorSet;
    }
    else
    {
        Logger::log("Error when parsing Color Set data!\n");
    }

    // Default - White skin Player (DPPt colors)
    return {
        .fieldSkinFace =      { 0.9490196, 0.76862746, 0.6627451, 1 },    // White skin
        .fieldSkinMouth =     { 0.9490196, 0.76862746, 0.6627451, 1 },    // White skin
        .fieldEyes =          { 0.72156864, 0.79607844, 0.9019608, 1 },   // Baby blue
        .fieldEyebrows =      { 0.23921569, 0.27450982, 0.36862746, 1 },  // Navy blue
        .fieldSkinBody =      { 0.9490196, 0.76862746, 0.6627451, 1 },    // White skin
        .fieldHair =          { 0.23921569, 0.27450982, 0.36862746, 1 },  // Navy blue

        .battleSkinFace =     { 0.9490196, 0.76862746, 0.6627451, 1 },    // White skin
        .battleHairExtra =    { 0.23921569, 0.27450982, 0.36862746, 1 },  // Navy blue
        .battleEyeLeft =      { 0.72156864, 0.79607844, 0.9019608, 1 },   // Baby blue
        .battleEyeRight =     { 0.72156864, 0.79607844, 0.9019608, 1 },   // Baby blue
        .battleSkinBody =     { 0.9490196, 0.76862746, 0.6627451, 1 },    // White skin
        .battleHair =         { 0.23921569, 0.27450982, 0.36862746, 1 },  // Navy blue
    };
}
