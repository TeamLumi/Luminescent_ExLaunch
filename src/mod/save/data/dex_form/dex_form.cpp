#include "helpers/fsHelper.h"
#include "save/save.h"

void loadDexFormsFromJson(const nn::json& saveFile) {
    if (saveFile.contains("lumi") && saveFile["lumi"].contains("dexForms")) {
        getCustomSaveData()->dexForms.FromJson(saveFile["lumi"]["dexForms"]);
    }
}

nn::json getDexFormsAsJson() {
    return getCustomSaveData()->dexForms.ToJson();
}
