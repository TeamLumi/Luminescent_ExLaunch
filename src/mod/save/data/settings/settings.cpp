#include "data/aspect_ratios.h"
#include "features/aspect_ratio.h"
#include "helpers/fsHelper.h"
#include "save/save.h"

void loadExtraSettingsFromJson(const nn::json& saveFile) {
    if (saveFile.contains("lumi") && saveFile["lumi"].contains("settings")) {
        getCustomSaveData()->settings.FromJson(saveFile["lumi"]["settings"]);

        // Apply saved aspect ratio
        auto& settings = getCustomSaveData()->settings;
        setTargetAspectRatio(aspectRatioEnumToFloat((int)settings.aspectRatio));
        reapplyAspectRatioPatches();
    }
}

nn::json getExtraSettingsAsJson() {
    return getCustomSaveData()->settings.ToJson();
}
