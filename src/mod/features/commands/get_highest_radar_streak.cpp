#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool GetHighestRadarStreak(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_GET_HIGHEST_RADAR_STREAK\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    auto radarRanks = PlayerWork::get_poketoreCountArray();

    // It will always be a length of 3, but accounting for manual json modification by giving a default of 0
    auto streak = (radarRanks.fields.data->max_length >= 1) ? radarRanks.fields.data->m_Items[0].fields.count : 0;
    SetWorkToValue(args->m_Items[1], streak);

    return true;
}