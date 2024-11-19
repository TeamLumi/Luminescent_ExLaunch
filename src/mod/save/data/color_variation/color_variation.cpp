#include "helpers/fsHelper.h"
#include "save/save.h"

void loadPlayerColorVariationFromJson(const nn::json& saveFile) {
    if (saveFile.contains("lumi") && saveFile["lumi"].contains("playerColorVariation")) {
        getCustomSaveData()->playerColorVariation.FromJson(saveFile["lumi"]["playerColorVariation"]);
    }
}

nn::json getPlayerColorVariationAsJson() {
    return getCustomSaveData()->playerColorVariation.ToJson();
}
