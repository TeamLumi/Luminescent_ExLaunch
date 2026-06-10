#pragma once

static constexpr const char* OVERWORLD_MP_OPTIONS[] = {
    "On",
    "Off",
};

static constexpr const char* OVERWORLD_MP_LABELS[] = {
    "SS_option_OverworldMP_01",
    "SS_option_OverworldMP_02",
};

constexpr int OVERWORLD_MP_OPTION_COUNT = sizeof(OVERWORLD_MP_OPTIONS) / sizeof(OVERWORLD_MP_OPTIONS[0]);
constexpr int OVERWORLD_MP_LABEL_COUNT = sizeof(OVERWORLD_MP_LABELS) / sizeof(OVERWORLD_MP_LABELS[0]);

static_assert(OVERWORLD_MP_OPTION_COUNT == OVERWORLD_MP_LABEL_COUNT);
