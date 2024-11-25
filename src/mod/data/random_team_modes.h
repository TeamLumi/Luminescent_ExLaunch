#pragma once

static constexpr const char* RANDOM_TEAM_MODES[] = {
    "Random Always",
    "Random Once",
    "Fixed Team 1",
};
static constexpr const char* RANDOM_TEAM_MODE_LABELS[] = {
    "SS_option_122",
    "SS_option_123",
    "SS_option_124",
};

constexpr int RANDOM_TEAM_MODE_COUNT = sizeof(RANDOM_TEAM_MODES) / sizeof(RANDOM_TEAM_MODES[0]);
constexpr int RANDOM_TEAM_MODE_LABEL_COUNT = sizeof(RANDOM_TEAM_MODE_LABELS) / sizeof(RANDOM_TEAM_MODE_LABELS[0]);

static_assert(RANDOM_TEAM_MODE_COUNT == RANDOM_TEAM_MODE_LABEL_COUNT);