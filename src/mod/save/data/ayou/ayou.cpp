#include "helpers/fsHelper.h"
#include "save/save.h"

void loadAYou(bool isBackup) {

    if (!isBackup && FsHelper::isFileExist(CustomSaveData::mainSaveName)) {
        nn::json jData = FsHelper::loadJsonFileFromPath(CustomSaveData::mainSaveName);
        if (jData.contains("lumi") && jData["lumi"].contains("ayou")) {
            getCustomSaveData()->main.FromJson(jData["lumi"]["ayou"]);
            Logger::log("Loaded Lumi_AYou!\n");
        }
    }

    else if (FsHelper::isFileExist(CustomSaveData::backupSaveName)) {
        nn::json jData = FsHelper::loadJsonFileFromPath(CustomSaveData::backupSaveName);
        if (jData.contains("lumi") && jData["lumi"].contains("ayou")) {
            getCustomSaveData()->main.FromJson(jData["lumi"]["ayou"]);
            Logger::log("Loaded Lumi_AYou_BK!\n");
        }
    }
}

nn::json saveAYou()
{
    return getCustomSaveData()->ayou.ToJson();
}
