#include "helpers/fsHelper.h"
#include "save/save.h"

static System::Boolean_array* cache_sysflags;

static System::Boolean_array* tmp_sysflags;

void loadSysFlags(bool isBackup)
{
    if (!isBackup && FsHelper::isFileExist(getCustomSaveData()->sysflags.fileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->sysflags.fileName);
        long expectedSize = getCustomSaveData()->sysflags.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->sysflags.fileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->sysflags.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_SysFlags!\n");
    }
    else if (FsHelper::isFileExist(getCustomSaveData()->sysflags.backupFileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->sysflags.backupFileName);
        long expectedSize = getCustomSaveData()->sysflags.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->sysflags.backupFileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->sysflags.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_SysFlags_BK!\n");
    }
}

void linkSysFlags(PlayerWork::Object* playerWork)
{
    // Create new array
    auto newSysFlags = System::Boolean_array::newArray(SysFlagCount);

    // Fill the new array with the custom save data
    memcpy(newSysFlags->m_Items, getCustomSaveData()->sysflags.items, sizeof(getCustomSaveData()->sysflags.items));

    // Cache the data that is in the vanilla save
    auto& savedata = playerWork->fields._saveData.fields;
    cache_sysflags = savedata.systemFlags;

    // Set the data in PlayerWork to our custom save data
    savedata.systemFlags = newSysFlags;
}

void unlinkSysFlags(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Copy PlayerWork data to our Custom save data
    memcpy(getCustomSaveData()->sysflags.items, savedata.systemFlags->m_Items, sizeof(getCustomSaveData()->sysflags.items));

    // Create a temp copy of the PlayerWork data
    tmp_sysflags = savedata.systemFlags;

    // Set PlayerWork to our cached data
    savedata.systemFlags = cache_sysflags;
}

void saveSysFlags(bool isMain, bool isBackup)
{
    char buffer[getCustomSaveData()->sysflags.GetByteCount()];
    getCustomSaveData()->sysflags.ToBytes((char*)buffer, 0);

    if (isMain)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->sysflags.fileName);
    if (isBackup)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->sysflags.backupFileName);
}

void relinkSysFlags(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Restore PlayerWork to our temp copy
    savedata.systemFlags = tmp_sysflags;
}