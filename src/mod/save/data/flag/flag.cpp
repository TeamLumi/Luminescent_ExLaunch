#include "helpers/fsHelper.h"
#include "save/save.h"

static System::Boolean_array* cache_flags;

static System::Boolean_array* tmp_flags;

void loadFlags(bool isBackup)
{
    if (!isBackup && FsHelper::isFileExist(getCustomSaveData()->flags.fileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->flags.fileName);
        long expectedSize = getCustomSaveData()->flags.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->flags.fileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->flags.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Flags!\n");
    }
    else if (FsHelper::isFileExist(getCustomSaveData()->flags.backupFileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->flags.backupFileName);
        long expectedSize = getCustomSaveData()->flags.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->flags.backupFileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->flags.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Flags_BK!\n");
    }
}

void linkFlags(PlayerWork::Object* playerWork)
{
    // Create new array
    auto newFlags = System::Boolean_array::newArray(FlagCount);

    // Fill the new array with the custom save data
    memcpy(newFlags->m_Items, getCustomSaveData()->flags.items, sizeof(getCustomSaveData()->flags.items));

    // Cache the data that is in the vanilla save
    auto& savedata = playerWork->fields._saveData.fields;
    cache_flags = savedata.boolValues;

    // Set the data in PlayerWork to our custom save data
    savedata.boolValues = newFlags;
}

void unlinkFlags(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Copy PlayerWork data to our Custom save data
    memcpy(getCustomSaveData()->flags.items, savedata.boolValues->m_Items, sizeof(getCustomSaveData()->flags.items));

    // Create a temp copy of the PlayerWork data
    tmp_flags = savedata.boolValues;

    // Set PlayerWork to our cached data
    savedata.boolValues = cache_flags;
}

void saveFlags(bool isMain, bool isBackup)
{
    char buffer[getCustomSaveData()->flags.GetByteCount()];
    getCustomSaveData()->flags.ToBytes((char*)buffer, 0);

    if (isMain)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->flags.fileName);
    if (isBackup)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->flags.backupFileName);
}

void relinkFlags(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Restore PlayerWork to our temp copy
    savedata.boolValues = tmp_flags;
}