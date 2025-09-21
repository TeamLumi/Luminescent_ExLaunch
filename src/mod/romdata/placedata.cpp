#include "exlaunch.hpp"

#include "externals/System/String.h"
#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/PlaceData.h"

#include "logger/logger.h"

const char* placeDataFolderPath = "rom:/Data/ExtraData/PlaceData/";

RomData::PlaceData GetExtraPlaceDataData(int32_t zoneID, System::String::Object* placeDataID)
{
    nn::string filePath(placeDataFolderPath);
    filePath.append("zone_" + nn::to_string(zoneID) + "_id_" + placeDataID->asCString().c_str() + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::PlaceData placeData = {};
        placeData = j.get<RomData::PlaceData>();

        return placeData;
    }
    else
    {
        Logger::log("Error when parsing PlaceData data!\n");
    }

    // Default
    return {
        .wearVariation = 0,
    };
}