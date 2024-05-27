#pragma once

static constexpr const char* GAME_MODES[] = {
        "SS_option_117", // 493 Mode
        "SS_option_118", // NatDex Mode
        "SS_option_119"  // Challenge Mode
};

constexpr int GAME_MODE_COUNT = sizeof(GAME_MODES) / sizeof(GAME_MODES[0]);

static constexpr const char* ON_OFF_NAMES[] = {
        "SS_option_007", // On
        "SS_option_008"  // Off
};

constexpr int ON_OFF_NAMES_COUNT = sizeof(ON_OFF_NAMES) / sizeof(ON_OFF_NAMES[0]);

static constexpr const char* RANDOM_TEAM_MODES[] = {
        "SS_option_122", // Random Always
        "SS_option_123", // Random Once
        "SS_option_124"  // Fixed Team 1
};

constexpr int RANDOM_TEAM_MODES_COUNT = sizeof(RANDOM_TEAM_MODES) / sizeof(RANDOM_TEAM_MODES[0]);

static constexpr const char* WINDOW_FRAME_TYPES[] = {
        "SS_option_080", // Type 1
        "SS_option_081", // Type 2
        "SS_option_082", // Type 3
        "SS_option_083", // Type 4
        "SS_option_084", // Type 5
        "SS_option_085", // Type 6
        "SS_option_086", // Type 7
        "SS_option_087", // Type 8
        "SS_option_088", // Type 9
        "SS_option_089", // Type 10
        "SS_option_090", // Type 11
        "SS_option_091", // Type 12
        "SS_option_092", // Type 13
        "SS_option_093", // Type 14
        "SS_option_094", // Type 15
        "SS_option_095", // Type 16
        "SS_option_096", // Type 17
        "SS_option_097", // Type 18
        "SS_option_098", // Type 19
        "SS_option_099", // Type 20
        "SS_option_101", // Type 21
        "SS_option_102", // Type 22
        "SS_option_103", // Type 23
        "SS_option_104"  // Type 24
};

constexpr int WINDOW_FRAME_TYPES_COUNT = sizeof(WINDOW_FRAME_TYPES) / sizeof(WINDOW_FRAME_TYPES[0]);
