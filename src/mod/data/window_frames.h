#pragma once

static constexpr const char* WINDOW_FRAMES[] = {
    "Type 1",
    "Type 2",
    "Type 3",
    "Type 4",
    "Type 5",
    "Type 6",
    "Type 7",
    "Type 8",
    "Type 9",
    "Type 10",
    "Type 11",
    "Type 12",
    "Type 13",
    "Type 14",
    "Type 15",
    "Type 16",
    "Type 17",
    "Type 18",
    "Type 19",
    "Type 20",
    "Type 21",
    "Type 22",
    "Type 23",
    "Type 24",
};

static constexpr const char* WINDOW_FRAME_LABELS[] = {
    "SS_option_ChangeWindow_01",
    "SS_option_ChangeWindow_02",
    "SS_option_ChangeWindow_03",
    "SS_option_ChangeWindow_04",
    "SS_option_ChangeWindow_05",
    "SS_option_ChangeWindow_06",
    "SS_option_ChangeWindow_07",
    "SS_option_ChangeWindow_08",
    "SS_option_ChangeWindow_09",
    "SS_option_ChangeWindow_10",
    "SS_option_ChangeWindow_11",
    "SS_option_ChangeWindow_12",
    "SS_option_ChangeWindow_13",
    "SS_option_ChangeWindow_14",
    "SS_option_ChangeWindow_15",
    "SS_option_ChangeWindow_16",
    "SS_option_ChangeWindow_17",
    "SS_option_ChangeWindow_18",
    "SS_option_ChangeWindow_19",
    "SS_option_ChangeWindow_20",
    "SS_option_ChangeWindow_21",
    "SS_option_ChangeWindow_22",
    "SS_option_ChangeWindow_23",
    "SS_option_ChangeWindow_24",
};

constexpr int WINDOW_FRAME_COUNT = sizeof(WINDOW_FRAMES) / sizeof(WINDOW_FRAMES[0]);
constexpr int WINDOW_FRAME_LABEL_COUNT = sizeof(WINDOW_FRAME_LABELS) / sizeof(WINDOW_FRAME_LABELS[0]);

static_assert(WINDOW_FRAME_COUNT == WINDOW_FRAME_LABEL_COUNT);