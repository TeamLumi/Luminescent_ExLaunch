#pragma once

static constexpr const char* ASPECT_RATIOS[] = {
    "16:9 (Auto)",
    "16:10",
    "21:9",
    "32:9",
};

static constexpr const char* ASPECT_RATIO_LABELS[] = {
    "SS_option_AspectRatio_01",
    "SS_option_AspectRatio_02",
    "SS_option_AspectRatio_03",
    "SS_option_AspectRatio_04",
};

static constexpr float ASPECT_RATIO_VALUES[] = {
    0.0f,       // 16:9 (Auto) - disabled, use default
    1.6f,       // 16:10
    2.3333f,    // 21:9
    3.5556f,    // 32:9
};

constexpr int ASPECT_RATIO_COUNT = sizeof(ASPECT_RATIOS) / sizeof(ASPECT_RATIOS[0]);
constexpr int ASPECT_RATIO_LABEL_COUNT = sizeof(ASPECT_RATIO_LABELS) / sizeof(ASPECT_RATIO_LABELS[0]);

static_assert(ASPECT_RATIO_COUNT == ASPECT_RATIO_LABEL_COUNT);

inline float aspectRatioEnumToFloat(int index) {
    if (index < 0 || index >= ASPECT_RATIO_COUNT) return 0.0f;
    return ASPECT_RATIO_VALUES[index];
}
