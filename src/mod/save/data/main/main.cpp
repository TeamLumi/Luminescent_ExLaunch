#include "helpers/fsHelper.h"
#include "save/save.h"

void loadMain(bool isBackup) {

    if (!isBackup && FsHelper::isFileExist(CustomSaveData::mainSaveName)) {
        nn::json jData = FsHelper::loadJsonFileFromPath(CustomSaveData::mainSaveName);
        if (jData.contains("lumi") && jData["lumi"].contains("main")) {
            getCustomSaveData()->main.FromJson(jData["lumi"]["main"]);
            Logger::log("Loaded Lumi_Main!\n");
        }
    }

    else if (FsHelper::isFileExist(CustomSaveData::backupSaveName)) {
        nn::json jData = FsHelper::loadJsonFileFromPath(CustomSaveData::backupSaveName);
        if (jData.contains("lumi") && jData["lumi"].contains("main")) {
            getCustomSaveData()->main.FromJson(jData["lumi"]["main"]);
            Logger::log("Loaded Lumi_Main_BK!\n");
        }
    }
}

nn::json saveMain()
{
    return getCustomSaveData()->main.ToJson();
}
