#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/LocalTrade.h"

#include "logger/logger.h"

const char* localTradeFolderPath = "rom:/Data/ExtraData/LocalTrades/";

void LogLocalTradeData(const RomData::LocalTrade& t)
{
    Logger::log("CURRENT LOCAL TRADE\n");

    Logger::log("IVs: ");
    for (int i=0; i<t.ivs.size(); i++)
        Logger::log("%d ", t.ivs[i]);
    Logger::log("\n");

    Logger::log("EVs: ");
    for (int i=0; i<t.evs.size(); i++)
        Logger::log("%d ", t.evs[i]);
    Logger::log("\n");

    Logger::log("Contest Stats: ");
    for (int i=0; i<t.contestStats.size(); i++)
        Logger::log("%d ", t.contestStats[i]);
    Logger::log("\n");

    Logger::log("Ball ID: %d\nForced Shiny: %d\n", t.ballId, t.forceShiny);
}

RomData::LocalTrade GetExtraLocalTradeData(int32_t tradeId)
{
    nn::string filePath(localTradeFolderPath);
    filePath.append("trade_" + nn::to_string(tradeId) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::LocalTrade tradeData = {};
        tradeData = j.get<RomData::LocalTrade>();
        LogLocalTradeData(tradeData);

        return tradeData;
    }
    else
    {
        Logger::log("Error when parsing Local Trade data!\n");
    }

    // Default: Random IVs, 0 EVs, 0 Contest Stats, Poké Ball, Not Forced Shiny
    return {
        .ivs = {},
        .evs = {},
        .contestStats = {},
        .ballId = 0,
        .forceShiny = false,
    };
}
