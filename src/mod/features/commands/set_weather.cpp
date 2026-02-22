#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/WeatherWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool SetWeather(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_SET_WEATHER\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t weather = GetWorkOrIntValue(args->m_Items[1]);
        Logger::log("Calling set_WeatherID with weatherId: %d\n", weather);
        WeatherWork::set_WeatherID(weather);
    }

    return true;
}