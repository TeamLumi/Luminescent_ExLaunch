#include "helpers/fsHelper.h"
#include "save/save.h"

void loadAYouFromJson(const nn::json& saveFile) {
    if (saveFile.contains("lumi") && saveFile["lumi"].contains("ayou")) {
        getCustomSaveData()->ayou.FromJson(saveFile["lumi"]["ayou"]);
    }
}

nn::json getAYouAsJson() {
    return getCustomSaveData()->ayou.ToJson();
}
