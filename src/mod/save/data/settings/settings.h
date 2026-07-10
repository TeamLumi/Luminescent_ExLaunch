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

    enum class FpsMode : int32_t {
        FPS_20 = 0,
        FPS_30 = 1,
        FPS_60 = 2,
        UNLIMITED = 3,
    };

    bool expShareEnabled;
    bool affectionEnabled;
    bool levelCapEnabled;
    bool shinyEggsEnabled;
    GameMode gameMode;
    RandomTeamMode randomTeamMode;
    FpsMode targetFpsMode;

    void Initialize() {
        expShareEnabled = false;
        affectionEnabled = false;
        levelCapEnabled = false;
        shinyEggsEnabled = false;
        gameMode = GameMode::_493;
        randomTeamMode = RandomTeamMode::RANDOM_ALWAYS;
        targetFpsMode = FpsMode::FPS_60;
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
                    {"targetFpsMode", targetFpsMode},
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
        targetFpsMode = settings["targetFpsMode"].get<FpsMode>();
    }
};

void loadExtraSettingsFromJson(const nn::json& saveFile);
nn::json getExtraSettingsAsJson();
