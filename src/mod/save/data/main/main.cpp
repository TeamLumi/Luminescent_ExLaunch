#include "helpers/fsHelper.h"
#include "save/save.h"

void loadMainFromJson(const nn::json& saveFile) {
    if (saveFile.contains("lumi") && saveFile["lumi"].contains("main")) {
        getCustomSaveData()->main.FromJson(saveFile["lumi"]["main"]);
    }
}

nn::json getMainAsJson() {
    return getCustomSaveData()->main.ToJson();
}
