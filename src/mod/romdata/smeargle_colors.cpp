#include "exlaunch.hpp"

#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/Random.h"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"
#include "romdata/data/SmeargleColor.h"

#include "logger/logger.h"

const char* smeargleColorFolderPath = "rom:/Data/ExtraData/Encounters/SmeargleColors/";

void LogSmeargleColorData(const RomData::SmeargleColor& c)
{
    Logger::log("CURRENT ZONE\n");
    Logger::log("Base color: %06X\n", c.baseColor);
    Logger::log("Hue Range: %f left, %f right\n", c.hueShiftNegative, c.hueShiftPositive);
    Logger::log("Saturation Range: %f left, %f right\n", c.saturationShiftNegative, c.saturationShiftPositive);
    Logger::log("Value Range: %f left, %f right\n", c.valueShiftNegative, c.valueShiftPositive);
    Logger::log("\n");
}

RomData::SmeargleColor GetSmeargleColorData(int32_t zoneID)
{
    nn::string filePath(smeargleColorFolderPath);
    filePath.append("zone_" + nn::to_string(zoneID) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::SmeargleColor smeargleColor = {};
        smeargleColor = j.get<RomData::SmeargleColor>();

        // Make the range values absolute, to make the math easier
        smeargleColor.hueShiftNegative = smeargleColor.hueShiftNegative < 0.0f ? smeargleColor.hueShiftNegative * -1.0f : smeargleColor.hueShiftNegative;
        smeargleColor.hueShiftPositive = smeargleColor.hueShiftPositive < 0.0f ? smeargleColor.hueShiftPositive * -1.0f : smeargleColor.hueShiftPositive;
        smeargleColor.saturationShiftNegative = smeargleColor.saturationShiftNegative < 0.0f ? smeargleColor.saturationShiftNegative * -1.0f : smeargleColor.saturationShiftNegative;
        smeargleColor.saturationShiftPositive = smeargleColor.saturationShiftPositive < 0.0f ? smeargleColor.saturationShiftPositive * -1.0f : smeargleColor.saturationShiftPositive;
        smeargleColor.valueShiftNegative = smeargleColor.valueShiftNegative < 0.0f ? smeargleColor.valueShiftNegative * -1.0f : smeargleColor.valueShiftNegative;
        smeargleColor.valueShiftPositive = smeargleColor.valueShiftPositive < 0.0f ? smeargleColor.valueShiftPositive * -1.0f : smeargleColor.valueShiftPositive;

        LogSmeargleColorData(smeargleColor);

        return smeargleColor;
    }
    else
    {
        Logger::log("Error when parsing Smeargle Color data!\n");
    }

    // Default, Invalid color
    return {
        .baseColor = -1,
        .hueShiftNegative = 0.0f,
        .hueShiftPositive = 0.0f,
        .saturationShiftNegative = 0.0f,
        .saturationShiftPositive = 0.0f,
        .valueShiftNegative = 0.0f,
        .valueShiftPositive = 0.0f,
    };
}

int32_t RollForSmeargleColor(int32_t zoneID)
{
    auto colorData = GetSmeargleColorData(zoneID);
    if (colorData.baseColor < 0)
        colorData.baseColor = UnityEngine::Random::Range(0, 0x1000000);

    auto baseColor = UnityEngine::Color::CreateFromRGB(colorData.baseColor);

    float hueShift = UnityEngine::Random::Range(-colorData.hueShiftNegative, colorData.hueShiftPositive);
    float saturationShift = UnityEngine::Random::Range(-colorData.saturationShiftNegative, colorData.saturationShiftPositive);
    float valueShift = UnityEngine::Random::Range(-colorData.valueShiftNegative, colorData.valueShiftPositive);

    return baseColor.HSVShift(hueShift, saturationShift, valueShift).ToHexCode();
}