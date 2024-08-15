#include "helpers/fsHelper.h"
#include "save/save.h"

static DPData::TR_BATTLE_DATA::Array* cache_trainers;

static DPData::TR_BATTLE_DATA::Array* tmp_trainers;

void loadTrainers(bool isBackup)
{
    if (!isBackup && FsHelper::isFileExist(getCustomSaveData()->trainers.fileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->trainers.fileName);
        long expectedSize = getCustomSaveData()->trainers.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->trainers.fileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->trainers.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Trainers!\n");
    }
    else if (FsHelper::isFileExist(getCustomSaveData()->trainers.backupFileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->trainers.backupFileName);
        long expectedSize = getCustomSaveData()->trainers.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->trainers.backupFileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->trainers.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Trainers_BK!\n");
    }
}

void linkTrainers(PlayerWork::Object* playerWork)
{
    // Create new array
    auto newTrainers = DPData::TR_BATTLE_DATA::newArray(TrainerCount);

    // Fill the new array with the custom save data
    memcpy(newTrainers->m_Items, getCustomSaveData()->trainers.items, sizeof(getCustomSaveData()->trainers.items));

    // Cache the data that is in the vanilla save
    auto& savedata = playerWork->fields._saveData.fields;
    cache_trainers = savedata.tr_battleData;

    // Set the data in PlayerWork to our custom save data
    savedata.tr_battleData = newTrainers;
}

void unlinkTrainers(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Copy PlayerWork data to our Custom save data
    memcpy(getCustomSaveData()->trainers.items, savedata.tr_battleData->m_Items, sizeof(getCustomSaveData()->trainers.items));

    // Create a temp copy of the PlayerWork data
    tmp_trainers = savedata.tr_battleData;

    // Set PlayerWork to our cached data
    savedata.tr_battleData = cache_trainers;
}

void saveTrainers(bool isMain, bool isBackup)
{
    char buffer[getCustomSaveData()->trainers.GetByteCount()];
    getCustomSaveData()->trainers.ToBytes((char*)buffer, 0);

    if (isMain)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->trainers.fileName);
    if (isBackup)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->trainers.backupFileName);
}

void relinkTrainers(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Restore PlayerWork to our temp copy
    savedata.tr_battleData = tmp_trainers;
}