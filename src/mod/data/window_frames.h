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
    "SS_option_080",
    "SS_option_081",
    "SS_option_082",
    "SS_option_083",
    "SS_option_084",
    "SS_option_085",
    "SS_option_086",
    "SS_option_087",
    "SS_option_088",
    "SS_option_089",
    "SS_option_090",
    "SS_option_091",
    "SS_option_092",
    "SS_option_093",
    "SS_option_094",
    "SS_option_095",
    "SS_option_096",
    "SS_option_097",
    "SS_option_098",
    "SS_option_099",
    "SS_option_101",
    "SS_option_102",
    "SS_option_103",
    "SS_option_104",
};

constexpr int WINDOW_FRAME_COUNT = sizeof(WINDOW_FRAMES) / sizeof(WINDOW_FRAMES[0]);
constexpr int WINDOW_FRAME_LABEL_COUNT = sizeof(WINDOW_FRAME_LABELS) / sizeof(WINDOW_FRAME_LABELS[0]);

static_assert(WINDOW_FRAME_COUNT == WINDOW_FRAME_LABEL_COUNT);