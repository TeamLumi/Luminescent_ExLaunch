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
        "FOG (Stubbed)",
        "Ashy",
        "Sandstorm",
        "Diamond Dust",
        "SPIRIT (Stubbed)",
        "MYSTIC (Stubbed)",
        "MIST1 (Stubbed)",
        "MIST2 (Stubbed)",
        "FLASH (Stubbed)",
        "SPARK_EFF (Stubbed)",
        "FOGS (Stubbed)",
        "FOGM (Stubbed)",
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