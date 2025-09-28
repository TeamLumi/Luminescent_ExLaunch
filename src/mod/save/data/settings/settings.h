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

    bool expShareEnabled;
    bool affectionEnabled;
    bool levelCapEnabled;
    bool shinyEggsEnabled;
    GameMode gameMode;
    RandomTeamMode randomTeamMode;

    void Initialize() {
        expShareEnabled = false;
        affectionEnabled = false;
        levelCapEnabled = false;
        shinyEggsEnabled = false;
        gameMode = GameMode::_493;
        randomTeamMode = RandomTeamMode::RANDOM_ALWAYS;
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
    }
};

void loadExtraSettingsFromJson(const nn::json& saveFile);
nn::json getExtraSettingsAsJson();
