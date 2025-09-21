#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/BodyColorSet.h"
#include "romdata/data/PlayerWearColorSet.h"
#include "romdata/data/WearColorSet.h"

#include "logger/logger.h"

const char* colorVariationBodyFolderPath = "rom:/Data/ExtraData/ColorVariation/Body/";
const char* colorVariationWearNPCFieldFolderPath = "rom:/Data/ExtraData/ColorVariation/Wear/NPC/Field/";
const char* colorVariationWearNPCBattleFolderPath = "rom:/Data/ExtraData/ColorVariation/Wear/NPC/Battle/";
const char* colorVariationWearPlayerFolderPath = "rom:/Data/ExtraData/ColorVariation/Wear/Player/";

RomData::BodyColorSet GetBodyColorSet(int32_t index)
{
    nn::string filePath(colorVariationBodyFolderPath);
    filePath.append("set_" + nn::to_string(index) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::BodyColorSet colorSet = {};
        colorSet = j.get<RomData::BodyColorSet>();

        return colorSet;
    }
    else
    {
        Logger::log("Error when parsing Body Color Set data!\n");
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

RomData::PlayerWearColorSet GetPlayerWearColorSet(int32_t outfit, int32_t index)
{
    Logger::log("outfit = %d, index = %d\n", outfit, index);
    nn::string filePath(colorVariationWearPlayerFolderPath);
    filePath.append("outfit_" + nn::to_string(outfit) + "_set_" + nn::to_string(index) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::PlayerWearColorSet colorSet = {};
        colorSet = j.get<RomData::PlayerWearColorSet>();

        return colorSet;
    }
    else
    {
        Logger::log("Error when parsing Player Wear Color Set data!\n");
    }

    // Default - ???
    return {
        .wearField {
            .color0 = { 0.8470588, 0.8470588, 0.8470588, 1 }, // ???
            .color1 = { 0.9019608, 0.6313726, 0.6509804, 1 }, // ???
            .color2 = { 0.7529412, 0.4156863, 0.4156863, 1 }, // ???
            .color3 = { 0.9019608, 0.6313726, 0.6509804, 1 }, // ???
            .color4 = { 0.8470588, 0.8470588, 0.8470588, 1 }, // ???
            .color5 = { 0.8470588, 0.8470588, 0.8470588, 1 }, // ???
            .color6 = { 1,         1,         1,         1 }, // ???
            .color7 = { 1,         1,         1,         1 }, // ???
            .color8 = { 1,         1,         1,         1 }, // ???
            .color9 = { 1,         1,         1,         1 }, // ???
        },
        .wearBattle {
            .color0 = { 0.8470588, 0.8470588, 0.8470588, 1 }, // ???
            .color1 = { 0.9019608, 0.6313726, 0.6509804, 1 }, // ???
            .color2 = { 0.7529412, 0.4156863, 0.4156863, 1 }, // ???
            .color3 = { 0.9019608, 0.6313726, 0.6509804, 1 }, // ???
            .color4 = { 0.8470588, 0.8470588, 0.8470588, 1 }, // ???
            .color5 = { 0.8470588, 0.8470588, 0.8470588, 1 }, // ???
            .color6 = { 1,         1,         1,         1 }, // ???
            .color7 = { 1,         1,         1,         1 }, // ???
            .color8 = { 1,         1,         1,         1 }, // ???
            .color9 = { 1,         1,         1,         1 }, // ???
        },
    };
}

RomData::WearColorSet GetNPCWearColorSet(int32_t ogi, int32_t index, bool battle)
{
    nn::string filePath(battle ? colorVariationWearNPCBattleFolderPath : colorVariationWearNPCFieldFolderPath);

    if (battle)
        filePath.append("type_" + nn::to_string(ogi) + "_set_" + nn::to_string(index) + ".json");
    else
        filePath.append("ogi_" + nn::to_string(ogi) + "_set_" + nn::to_string(index) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::WearColorSet colorSet = {};
        colorSet = j.get<RomData::WearColorSet>();

        return colorSet;
    }
    else
    {
        Logger::log("Error when parsing NPC Wear Color Set data!\n");
    }

    // Default - ???
    return {
        .color0 = { 0.8470588, 0.8470588, 0.8470588, 1 }, // ???
        .color1 = { 0.9019608, 0.6313726, 0.6509804, 1 }, // ???
        .color2 = { 0.7529412, 0.4156863, 0.4156863, 1 }, // ???
        .color3 = { 0.9019608, 0.6313726, 0.6509804, 1 }, // ???
        .color4 = { 0.8470588, 0.8470588, 0.8470588, 1 }, // ???
        .color5 = { 0.8470588, 0.8470588, 0.8470588, 1 }, // ???
        .color6 = { 1,         1,         1,         1 }, // ???
        .color7 = { 1,         1,         1,         1 }, // ???
        .color8 = { 1,         1,         1,         1 }, // ???
        .color9 = { 1,         1,         1,         1 }, // ???
    };
}