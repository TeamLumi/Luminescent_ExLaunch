#include "helpers/fsHelper.h"
#include "save/save.h"

void loadExtraSettingsFromJson(const nn::json& saveFile) {
    if (saveFile.contains("lumi") && saveFile["lumi"].contains("settings")) {
        getCustomSaveData()->settings.FromJson(saveFile["lumi"]["settings"]);
    }
}

nn::json getExtraSettingsAsJson() {
    return getCustomSaveData()->settings.ToJson();
}
