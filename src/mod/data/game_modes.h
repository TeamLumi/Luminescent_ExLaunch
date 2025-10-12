#pragma once

static constexpr const char* GAME_MODES[] = {
    "493 Mode",
    "NatDex Mode",
    "Challenge Mode",
};

static constexpr const char* GAME_MODE_LABELS[] = {
    "SS_option_GameMode_01",
    "SS_option_GameMode_02",
    "SS_option_GameMode_03",
};

constexpr int GAME_MODE_COUNT = sizeof(GAME_MODES) / sizeof(GAME_MODES[0]);
constexpr int GAME_MODE_LABEL_COUNT = sizeof(GAME_MODE_LABELS) / sizeof(GAME_MODE_LABELS[0]);

static_assert(GAME_MODE_COUNT == GAME_MODE_LABEL_COUNT);