#include "helpers/fsHelper.h"
#include "save/save.h"

void loadSymbolEncountersFromJson(const nn::json& saveFile) {
    if (saveFile.contains("lumi") && saveFile["lumi"].contains("symbolEncounters")) {
        getCustomSaveData()->symbolEncounters.FromJson(saveFile["lumi"]["symbolEncounters"]);
    }
}

nn::json getSymbolEncountersAsJson() {
    return getCustomSaveData()->symbolEncounters.ToJson();
}
