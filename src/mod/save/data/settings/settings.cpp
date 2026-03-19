#include "data/settings.h"
#include "data/utils.h"
#include "helpers/fsHelper.h"
#include "save/save.h"
#include "utils/aspect_ratio.h"

void loadExtraSettingsFromJson(const nn::json& saveFile) {
    if (saveFile.contains("lumi") && saveFile["lumi"].contains("settings")) {
        getCustomSaveData()->settings.FromJson(saveFile["lumi"]["settings"]);

        // Apply saved aspect ratio
        auto& settings = getCustomSaveData()->settings;
        applyAspectRatioSetting(array_index(SETTINGS, "Aspect Ratio"), (int32_t)settings.aspectRatio);
    }
}

nn::json getExtraSettingsAsJson() {
    return getCustomSaveData()->settings.ToJson();
}
