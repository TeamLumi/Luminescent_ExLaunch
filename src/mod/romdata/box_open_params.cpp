#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/BoxOpenParam.h"

#include "logger/logger.h"

const char* boxOpenParamFolderPath = "rom:/Data/ExtraData/UI/BoxOpenParam/";

void LogBoxOpenParam(const RomData::BoxOpenParam& p)
{
    Logger::log("SELECTED BOX OPEN PARAM\n");
    Logger::log("Trade ID: %d\n", p.tradeId);
}

RomData::BoxOpenParam GetExtraBoxOpenParamData(int32_t paramId)
{
    nn::string filePath(boxOpenParamFolderPath);
    filePath.append("param_" + nn::to_string(paramId) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::BoxOpenParam boxOpenParam = {};
        boxOpenParam = j.get<RomData::BoxOpenParam>();

        return boxOpenParam;
    }
    else
    {
        Logger::log("Error when parsing BoxOpenParam data!\n");
    }

    // Default: No associated local trade
    return {
        .tradeId = -1,
    };
}