#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/Trainer.h"
#include "romdata/data/TrainerType.h"

#include "logger/logger.h"

const char* trainerFolderPath = "rom:/Data/ExtraData/Trainers/Trainers/";
const char* trainerTypeFolderPath = "rom:/Data/ExtraData/Trainers/Types/";

RomData::Trainer GetExtraTrainerData(int32_t id)
{
    nn::string filePath(trainerFolderPath);
    filePath.append("trainer_" + nn::to_string(id) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::Trainer trainer = {};
        trainer = j.get<RomData::Trainer>();

        return trainer;
    }
    else
    {
        Logger::log("Error when parsing Trainer data!\n");
    }

    // Default
    return {
        .wearVariation = 0,
    };
}