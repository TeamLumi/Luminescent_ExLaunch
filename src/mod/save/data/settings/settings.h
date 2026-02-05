#pragma once

#include "externals/PlayerWork.h"

#include "logger/logger.h"
#include "memory/json.h"

struct ExtraSettingsSaveData {
    enum class GameMode : int32_t {
        _493 = 0,
        NATDEX = 1,
        CHALLENGE = 2,
    };

    enum class RandomTeamMode : int32_t {
        RANDOM_ALWAYS = 0,
        RANDOM_ONCE = 1,
        FIXED_TEAM_1 = 2,
    };

    enum class AspectRatio : int32_t {
        AUTO_16_9 = 0,
        _16_10 = 1,
        _21_9 = 2,
        _32_9 = 3,
    };

    bool expShareEnabled;
    bool affectionEnabled;
    bool levelCapEnabled;
    bool shinyEggsEnabled;
    GameMode gameMode;
    RandomTeamMode randomTeamMode;
    AspectRatio aspectRatio;

    void Initialize() {
        expShareEnabled = false;
        affectionEnabled = false;
        levelCapEnabled = false;
        shinyEggsEnabled = false;
        gameMode = GameMode::_493;
        randomTeamMode = RandomTeamMode::RANDOM_ALWAYS;
        aspectRatio = AspectRatio::AUTO_16_9;
    }

    [[nodiscard]] nn::json ToJson() const {
        return {
            {"settings", {
                    {"expShareEnabled", expShareEnabled},
                    {"affectionEnabled", affectionEnabled},
                    {"levelCapEnabled", levelCapEnabled},
                    {"shinyEggsEnabled", shinyEggsEnabled},
                    {"gameMode", gameMode},
                    {"randomTeamMode", randomTeamMode},
                    {"aspectRatio", aspectRatio},
            }}
        };
    }

    void FromJson(const nn::json& settings) {
        Initialize();
        expShareEnabled = settings["expShareEnabled"].get<bool>();
        affectionEnabled = settings["affectionEnabled"].get<bool>();
        levelCapEnabled = settings["levelCapEnabled"].get<bool>();
        shinyEggsEnabled = settings["shinyEggsEnabled"].get<bool>();
        gameMode = settings["gameMode"].get<GameMode>();
        randomTeamMode = settings["randomTeamMode"].get<RandomTeamMode>();
        if (settings.contains("aspectRatio"))
            aspectRatio = settings["aspectRatio"].get<AspectRatio>();
    }
};

void loadExtraSettingsFromJson(const nn::json& saveFile);
nn::json getExtraSettingsAsJson();
