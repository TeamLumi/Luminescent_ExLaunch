#pragma once

static constexpr const char* FPS_MODES[] = {
    "20 FPS",
    "30 FPS",
    "60 FPS",
    "Unlimited",
};

static constexpr const char* FPS_MODE_LABELS[] = {
    "SS_option_TargetFPS_01",
    "SS_option_TargetFPS_02",
    "SS_option_TargetFPS_03",
    "SS_option_TargetFPS_04",
};

static constexpr int32_t FPS_MODE_VALUES[] = {
    20,
    30,
    60,
    -1,
};

constexpr int FPS_MODE_COUNT = sizeof(FPS_MODES) / sizeof(FPS_MODES[0]);
constexpr int FPS_MODE_LABEL_COUNT = sizeof(FPS_MODE_LABELS) / sizeof(FPS_MODE_LABELS[0]);

static_assert(FPS_MODE_COUNT == FPS_MODE_LABEL_COUNT);
