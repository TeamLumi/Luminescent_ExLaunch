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
    "Trainer Sets",
    "Team Randomization",
    "Overworld Multiplayer",
};

static constexpr const char *SETTING_DESCRIPTION_LABELS[] = {
    "SS_option_TextSpeed_Desc",
    "SS_option_Kanji_Desc",
    "SS_option_ChangeWindow_Desc",
    "SS_option_BattleEffects_Desc",
    "SS_option_BattleStyle_Desc",
    "SS_option_Boxes_Desc",
    "SS_option_GiveNickname_Desc",
    "SS_option_Gyroscope_Desc",
    "SS_option_FossilShake_Desc",
    "SS_option_Autosave_Desc",
    "SS_option_ShowNickname_Desc",
    "SS_option_BGM_Desc",
    "SS_option_SoundEffect_Desc",
    "SS_option_Cries_Desc",
    "SS_option_ExpShare_Desc",
    "SS_option_Affection_Desc",
    "SS_option_LevelCap_Desc",
    "SS_option_ShinyEgg_Desc",
    "SS_option_GameMode_Desc",
    "SS_option_TeamRandom_Desc",
    "SS_option_OverworldMP_Desc",
};

const int SETTING_COUNT = sizeof(SETTINGS) / sizeof(SETTINGS[0]);
constexpr int SETTING_DESCRIPTION_LABEL_COUNT = sizeof(SETTING_DESCRIPTION_LABELS) / sizeof(SETTING_DESCRIPTION_LABELS[0]);

static_assert(SETTING_COUNT == SETTING_DESCRIPTION_LABEL_COUNT);

