#pragma once

static constexpr const char* FIELD_WEATHERS[] = {
    "Sunny",
    "Cloudy",
    "Rain",
    "Sideways Rain",
    "Thunderstorm",
    "Snow",
    "Snowstorm",
    "Heavy Snowstorm",
    "FOG",
    "Ashy",
    "Sandstorm",
    "Diamond Dust",
    "SPIRIT",
    "MYSTIC",
    "MIST1",
    "MIST2",
    "FLASH",
    "SPARK_EFF",
    "FOGS",
    "FOGM",
};

constexpr int FIELD_WEATHER_COUNT = sizeof(FIELD_WEATHERS) / sizeof(FIELD_WEATHERS[0]);

static constexpr const char* BATTLE_WEATHERS[] = {
    "-BATTLE WEATHER ZERO-",
    "Harsh Sunlight",
    "Rain",
    "Hail",
    "Sandstorm",
    "Heavy Rain",
    "Extremely Harsh Sunlight",
    "Strong Winds",
};

constexpr int BATTLE_WEATHER_COUNT = sizeof(BATTLE_WEATHERS) / sizeof(BATTLE_WEATHERS[0]);
