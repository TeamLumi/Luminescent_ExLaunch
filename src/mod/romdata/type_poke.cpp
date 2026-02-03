#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"
#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"
#include "romdata/data/TypePokeData.h"
#include "externals/UnityEngine/Random.h"

#include "logger/logger.h"

const char* typePokesFolderPath = "rom:/Data/ExtraData/Encounters/TypePokes/";

RomData::TypePokeSlot GetTypePokeSlot(int32_t typeID)
{
    nn::string filePath(typePokesFolderPath);
    filePath.append("type_" + nn::to_string(typeID) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::TypePokeList typePokeList = {};
        typePokeList = j.get<RomData::TypePokeList>();

        if (typePokeList.slots.size() > 0)
        {
            // Get a random slot from the list
            int32_t slot = UnityEngine::Random::Range(0, (int32_t)typePokeList.slots.size());
            Logger::log("GetTypePokeSlot: Type %d, Slot %d/%d, MonsNo %d, FormNo %d\n",
                typeID, slot, (int32_t)typePokeList.slots.size(),
                typePokeList.slots[slot].monsNo, typePokeList.slots[slot].formNo);
            return typePokeList.slots[slot];
        }
        else
        {
            Logger::log("GetTypePokeSlot: Type %d has no slots!\n", typeID);
        }
    }
    else
    {
        Logger::log("GetTypePokeSlot: Error parsing Type Poke data for type %d!\n", typeID);
    }

    // Default - return Bulbasaur
    return {
        .monsNo = 1,
        .formNo = 0,
    };
}

int32_t GetTypePokeCount(int32_t typeID)
{
    nn::string filePath(typePokesFolderPath);
    filePath.append("type_" + nn::to_string(typeID) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::TypePokeList typePokeList = {};
        typePokeList = j.get<RomData::TypePokeList>();
        return (int32_t)typePokeList.slots.size();
    }

    return 0;
}
