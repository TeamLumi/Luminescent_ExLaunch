#include "helpers/fsHelper.h"
#include "save/save.h"

static System::Int32_array* cache_works;

static System::Int32_array* tmp_works;

void loadWorks(bool isBackup)
{
    if (!isBackup && FsHelper::isFileExist(getCustomSaveData()->works.fileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->works.fileName);
        long expectedSize = getCustomSaveData()->works.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->works.fileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->works.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Works!\n");
    }
    else if (FsHelper::isFileExist(getCustomSaveData()->works.backupFileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->works.backupFileName);
        long expectedSize = getCustomSaveData()->works.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->works.backupFileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->works.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Works_BK!\n");
    }
}

void linkWorks(PlayerWork::Object* playerWork)
{
    // Create new array
    auto newWorks = System::Int32_array::newArray(WorkCount);

    // Fill the new array with the custom save data
    memcpy(newWorks->m_Items, getCustomSaveData()->works.items, sizeof(getCustomSaveData()->works.items));

    // Cache the data that is in the vanilla save
    auto& savedata = playerWork->fields._saveData.fields;
    cache_works = savedata.intValues;

    // Set the data in PlayerWork to our custom save data
    savedata.intValues = newWorks;
}

void unlinkWorks(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Copy PlayerWork data to our Custom save data
    memcpy(getCustomSaveData()->works.items, savedata.intValues->m_Items, sizeof(getCustomSaveData()->works.items));

    // Create a temp copy of the PlayerWork data
    tmp_works = savedata.intValues;

    // Set PlayerWork to our cached data
    savedata.intValues = cache_works;
}

void saveWorks(bool isMain, bool isBackup)
{
    char buffer[getCustomSaveData()->works.GetByteCount()];
    getCustomSaveData()->works.ToBytes((char*)buffer, 0);

    if (isMain)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->works.fileName);
    if (isBackup)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->works.backupFileName);
}

void relinkWorks(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Restore PlayerWork to our temp copy
    savedata.intValues = tmp_works;
}