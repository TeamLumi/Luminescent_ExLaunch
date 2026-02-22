#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/ZoneRates.h"

#include "externals/UnityEngine/Random.h"

#include "logger/logger.h"

const char* zoneFormsFolderPath = "rom:/Data/ExtraData/Encounters/ZoneForms/";
const char* zoneVariantsFolderPath = "rom:/Data/ExtraData/Encounters/ZoneVariants/";

int32_t Roll(nn::vector<uint32_t>* rates)
{
    int32_t sum = 0;
    for (auto& rate : *rates)
        sum += rate;

    auto roll = UnityEngine::Random::Range(0, sum);

    int32_t currentSum = 0;
    for (size_t i=0; i<(*rates).size(); i++)
    {
        if (roll < currentSum + (int32_t)(*rates)[i])
            return i;

        currentSum += (*rates)[i];
    }

    // Should only happen if rates is empty or all rates are 0
    return 0;
}

nn::vector<uint32_t> GetFormRates(int32_t monsno, int32_t zoneID)
{
    nn::string filePath(zoneFormsFolderPath);
    filePath.append("monsno_" + nn::to_string(monsno) + "_zone_" + nn::to_string(zoneID) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::ZoneRates zoneRates = {};
        zoneRates = j.get<RomData::ZoneRates>();

        return zoneRates.rates;
    }

    // Default - Only Form 0
    return { 100 };
}

int32_t RollForForm(int32_t monsno, int32_t zoneID)
{
    auto rates = GetFormRates(monsno, zoneID);
    return Roll(&rates);
}

nn::vector<uint32_t> GetVariantRates(int32_t monsno, int32_t formno, int32_t zoneID)
{
    nn::string filePath(zoneVariantsFolderPath);
    filePath.append("monsno_" + nn::to_string(monsno) + "_formno_" + nn::to_string(formno) + "_zone_" + nn::to_string(zoneID) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::ZoneRates zoneRates = {};
        zoneRates = j.get<RomData::ZoneRates>();

        return zoneRates.rates;
    }

    // Default - Only Variant 0
    return { 100 };
}

int32_t RollForVariant(int32_t monsno, int32_t formno, int32_t zoneID)
{
    auto rates = GetVariantRates(monsno, formno, zoneID);
    return Roll(&rates);
}