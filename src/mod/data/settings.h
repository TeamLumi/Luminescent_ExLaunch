#pragma once

static constexpr const char *SETTINGS[] = {
    "Text Speed",
    "Kanji Mode",
    "Change Window",
    "Battle Effects",
    "Battle Style",
    "Send to Boxes",
    "Give Nicknames",
    "Gyroscope",
    "Camera Shake",
    "Autosave",
    "Show Nicknames",
    "Background Music",
    "Sound Effects",
    "Pokémon Cries",
    "EXP Share",
    "Affection",
    "Level Cap",
    "Visible Shiny Eggs",
    "Game Mode",
    "Team Randomization",
};

static constexpr const char *SETTING_DESCRIPTION_LABELS[] = {
    "SS_option_005",
    "SS_option_017",
    "SS_option_100",
    "SS_option_009",
    "SS_option_013",
    "SS_option_025",
    "SS_option_074",
    "SS_option_033",
    "SS_option_108",
    "SS_option_035",
    "SS_option_070",
    "SS_option_041",
    "SS_option_043",
    "SS_option_045",
    "SS_option_113",
    "SS_option_112",
    "SS_option_114",
    "SS_option_126",
    "SS_option_116",
    "SS_option_121",
};

const int SETTING_COUNT = sizeof(SETTINGS) / sizeof(SETTINGS[0]);
constexpr int SETTING_DESCRIPTION_LABEL_COUNT = sizeof(SETTING_DESCRIPTION_LABELS) / sizeof(SETTING_DESCRIPTION_LABELS[0]);

static_assert(SETTING_COUNT == SETTING_DESCRIPTION_LABEL_COUNT);

